// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Pickable/Weapons/SimpleWeaponInstance.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

USimpleWeaponInstance::USimpleWeaponInstance()
{
}

void USimpleWeaponInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, CurrentCartridge);
}

void USimpleWeaponInstance::WeaponTick(float DeltaSeconds)
{
	const bool bMinSpread = UpdateSpread(DeltaSeconds);
	const bool bMinMultipliers = UpdateMultipliers(DeltaSeconds);

	//只有武器散射和倍率恢复到一定状态才能有首发精准
	bHasFirstShotAccuracy = bAllowFirstShotAccuracy && bMinSpread && bMinMultipliers;
}

void USimpleWeaponInstance::OnEquipped(UObject* InInstigator)
{
	float MinHeatRange, MaxHeatRange;
	ComputeHeatRange(MinHeatRange, MaxHeatRange);
	//防止武器过热过冷
	CurrentHeat = (MinHeatRange + MaxHeatRange) * 0.5f;

	//Eval求该曲线在指定时刻的值
	CurrentSpreadAngle = HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);

	//设置散射倍率无缩放
	CurrentSpreadAngleMultiplier = 1.f;
	StandingStillMultiplier = 1.f;
	CrouchingMultiplier = 1.f;

	SetAnimationParamsOnEquipped(InInstigator);
}

void USimpleWeaponInstance::OnUnEquipped(UObject* InInstigator)
{
	SetAnimationParamsOnUnEquipped(InInstigator);
}

void USimpleWeaponInstance::AddSpread()
{
	//每发增加武器热量
	const float HeatPerShot = HeatToHeatPerShotCurve.GetRichCurveConst()->Eval(CurrentHeat);
	CurrentHeat = ClampHeat(CurrentHeat + HeatPerShot);

	//计算武器散射
	CurrentSpreadAngle = HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);
}

void USimpleWeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	LastFireTime = World->GetDeltaSeconds();
}

bool USimpleWeaponInstance::UpdateSpread(float DeltaSeconds)
{
	//距离上次开火的时间
	const float TimeSinceFire = GetWorld() ? GetWorld()->TimeSince(LastFireTime) : 0.f;
	//如果超过了冷却时间
	if (TimeSinceFire > SpreadRecoveryCoolDownDelay)
	{
		//冷却速度
		const float CoolDownRate = HeatToCoolDownPerSecondCurve.GetRichCurveConst()->Eval(CurrentHeat);
		//冷却
		CurrentHeat = ClampHeat(CurrentHeat - CoolDownRate * DeltaSeconds);

		//计算武器散射
		CurrentSpreadAngle = HeatToSpreadAngleCurve.GetRichCurveConst()->Eval(CurrentHeat);
	}

	//武器散射范围
	float MinSpread, MaxSpread;
	ComputeSpreadRange(MinSpread, MaxSpread);

	//散射和最低散射做比较
	return FMath::IsNearlyEqual(CurrentSpreadAngle, MinSpread,KINDA_SMALL_NUMBER);
}

bool USimpleWeaponInstance::UpdateMultipliers(float DeltaSeconds)
{
	const float MultiplierNearlyEqualThreshlod = 0.05f;

	ACharacter* PlayerCharacter = GetCharacter();
	UCharacterMovementComponent* CharMoveComp = PlayerCharacter ? PlayerCharacter->GetCharacterMovement() : nullptr;
	const float CharacterSpeed = PlayerCharacter ? PlayerCharacter->GetVelocity().Size() : 0.f;

	const float MovementTargetAngle = FMath::GetMappedRangeValueClamped(
		FVector2D(StandingStillSpeedThreshold, StandingStillSpeedThreshold + StandingStillToMovingSpeedRange),
		FVector2D(SpreadAngleMultiplier_StandingStill, 1.f),
		CharacterSpeed);

	return false;
}

void USimpleWeaponInstance::CartridgeCost(int32 CostCounts)
{
	check(CostCounts<=CurrentCartridge);

	CurrentCartridge -= CostCounts;
}

void USimpleWeaponInstance::ReloadCartridge(int32 CartridgeCounts)
{
	CurrentCartridge += CartridgeCounts;

	check(CurrentCartridge<=ClipSize);
}

ACharacter* USimpleWeaponInstance::GetCharacter() const
{
	return Cast<ACharacter>(Instigator);
}

void USimpleWeaponInstance::ComputeSpreadRange(float& MinSpread, float& MaxSpread)
{
	HeatToSpreadAngleCurve.GetRichCurveConst()->GetValueRange(MinSpread, MaxSpread);
}

void USimpleWeaponInstance::ComputeHeatRange(float& MinHeat, float& MaxHeat)
{
	float Min1, Max1;
	HeatToHeatPerShotCurve.GetRichCurveConst()->GetValueRange(Min1, Max1);

	float Min2, Max2;
	HeatToCoolDownPerSecondCurve.GetRichCurveConst()->GetValueRange(Min2, Max2);

	float Min3, Max3;
	HeatToSpreadAngleCurve.GetRichCurveConst()->GetValueRange(Min3, Max3);

	MinHeat = FMath::Min(Min1, FMath::Min(Min2, Min3));
	MaxHeat = FMath::Max(Max1, FMath::Max(Max2, Max3));
}

class UWorld* USimpleWeaponInstance::GetWorld() const
{
	if (Instigator)
	{
		return Instigator->GetWorld();
	}

	return nullptr;
}
