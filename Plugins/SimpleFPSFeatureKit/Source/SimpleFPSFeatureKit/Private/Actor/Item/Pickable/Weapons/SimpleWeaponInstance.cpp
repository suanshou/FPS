// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Pickable/Weapons/SimpleWeaponInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	//散射倍率近似误差
	const float MultiplierNearlyEqualThreshlod = 0.05f;

	//角色
	ACharacter* PlayerCharacter = GetCharacter();
	//角色移动组件
	UCharacterMovementComponent* CharMoveComp = PlayerCharacter ? PlayerCharacter->GetCharacterMovement() : nullptr;
	//角色移动速度
	const float CharacterSpeed = PlayerCharacter ? PlayerCharacter->GetVelocity().Size() : 0.f;

	//角色移动时
	//散射目标值
	//GetMappedRangeValueClamped对于给定的参数3，返回对应的百分比
	const float MovementTargetAngle = FMath::GetMappedRangeValueClamped(
		FVector2D(StandingStillSpeedThreshold, StandingStillSpeedThreshold + StandingStillToMovingSpeedRange),
		FVector2D(SpreadAngleMultiplier_StandingStill, 1.f),
		CharacterSpeed);

	//站立时的散射倍率
	//FInterpTo贝塞尔曲线做平滑过渡
	StandingStillMultiplier = FMath::FInterpTo(StandingStillMultiplier,
	                                           MovementTargetAngle,
	                                           DeltaSeconds,
	                                           TransitionRate_StandingStill);
	//是否是站立时的最小倍率
	const bool bStandingStillMultiplierAtMin = FMath::IsNearlyEqual(StandingStillMultiplier,
	                                                                SpreadAngleMultiplier_StandingStill,
	                                                                MultiplierNearlyEqualThreshlod);

	//角色蹲下时
	//是否蹲下
	const bool bIsCrouching = (CharMoveComp != nullptr) && CharMoveComp->IsCrouching();
	//蹲下时的散射倍率
	const float CrouchingTargetValue = bIsCrouching ? SpreadAngleMultiplier_Crouching : 1.f;
	CrouchingMultiplier = FMath::FInterpTo(CrouchingMultiplier,
	                                       CrouchingTargetValue,
	                                       DeltaSeconds,
	                                       TransitionRate_Crouching);

	//是否是蹲下时的最小倍率
	const bool bCrouchingMultiplierAtMin = FMath::IsNearlyEqual(CrouchingMultiplier,
	                                                            SpreadAngleMultiplier_Crouching,
	                                                            MultiplierNearlyEqualThreshlod);

	//瞄准时
	float AimingAlpha = 0.f;
	const float AimingMultiplier = FMath::GetMappedRangeValueClamped(
		FVector2D(0.f, 1.f),
		FVector2D(1.f, SpreadAngleMultiplier_Crouching),
		AimingAlpha);
	const bool bAimingMultiplierAtMin = FMath::IsNearlyEqual(AimingMultiplier,
	                                                         SpreadAngleMultiplier_Aiming,
	                                                         MultiplierNearlyEqualThreshlod);

	//叠加所有的倍率
	CurrentSpreadAngleMultiplier = StandingStillMultiplier * CrouchingMultiplier * AimingMultiplier;

	//是否都在最小值
	//用于首发精准
	return bStandingStillMultiplierAtMin && bCrouchingMultiplierAtMin && bAimingMultiplierAtMin;
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
