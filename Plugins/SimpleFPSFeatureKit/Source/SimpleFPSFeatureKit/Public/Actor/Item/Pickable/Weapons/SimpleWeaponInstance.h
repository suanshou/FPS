// Fill out your copyright notice in the Description page of Project Settings.
//记录子弹数量等实例信息

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SimpleFPSFeatureKitType.h"
#include "SimpleWeaponInstance.generated.h"

class UGameplayAbility;
enum class ESimpleWeaponSlot : uint8;
//class UGameplayAbility;

USTRUCT(BlueprintType)
struct SIMPLEFPSFEATUREKIT_API FWeaponBindingAbility
{
	GENERATED_USTRUCT_BODY()

	//武器技能
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Ability")
	TSubclassOf<UGameplayAbility> WeaponAbility;

	//对应的技能标签
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Ability")
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct SIMPLEFPSFEATUREKIT_API FSimpleWeaponEquippedMontage
{
	GENERATED_USTRUCT_BODY()

	FSimpleWeaponEquippedMontage()
	{
		PlayRate = 1.f;
		bStopAllMontage = true;
	}

	//播放的蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	TObjectPtr<UAnimMontage> AnimMontage;

	//播放蒙太奇的片段
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	FName TransactionName;

	//播放速率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	float PlayRate;

	//是否停止所有蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Equipped Montage")
	bool bStopAllMontage;
};

UCLASS(BlueprintType, Blueprintable)
class SIMPLEFPSFEATUREKIT_API USimpleWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	USimpleWeaponInstance();

protected:
	//武器类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	ESimpleWeaponSlot WeaponSlot = ESimpleWeaponSlot::WS_INVALID;

	//武器槽名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FName InventorySlotName;

	//插槽相对位置矫正
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FTransform InventoryRelativeTransform;

	//武器技能
	//通过按键激活GAS
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	TArray<FWeaponBindingAbility> BindingAbilities;

	//装备动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FSimpleWeaponEquippedMontage EquippedMontage;

	//卸载动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FSimpleWeaponEquippedMontage UnEquippedMontage;

	//初始子弹数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	int32 CartridgeCountsOnSpawn = 30;

	//弹夹容量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	int32 ClipSize = 30;

	//每发子弹的弹丸数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	int32 BulletsPerCartridge = 1;

	//最大伤害范围
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	float MaxDamageRange = 30000.f;

	//子弹检测范围
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	float BulletRadius = 5.f;

	//伤害衰减曲线
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	FRuntimeFloatCurve DistanceDamageFalloffCurve;

	//材质对应的伤害倍率
	//处理爆头伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Basic")
	TMap<FName, float> MaterialDamageMultiplier;

	//武器散射和能量系统
	//散射角指数，指数越大散射越极端
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float SpreadExponent = 1.f;

	//武器对散射的影响
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	FRuntimeFloatCurve HeatToSpreadAngleCurve;

	//每发产生的热量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	FRuntimeFloatCurve HeatToHeatPerShotCurve;

	//每秒的冷却
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	FRuntimeFloatCurve HeatToCoolDownPerSecondCurve;

	//冷却后散射的恢复时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	float SpreadRecoveryCoolDownDelay = 0.2f;

	//开启首发精准
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Fire Params")
	bool bAllowFirstShotAccuracy = true;

	//开镜散射倍率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Multiplier")
	float SpreadAngleMultiplier_Aiming = 1.f;

	//站立散射倍率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Multiplier")
	float SpreadAngleMultiplier_StandingStill = 1.f;

	//站立的散射平滑过渡率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Multiplier")
	float TransitionRate_StandingStill = 5.f;

	//判定为站立的阈值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Multiplier")
	float StandingStillSpeedThreshold = 20.f;

	//站立到移动的阈值
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Multiplier")
	float StandingStillToMovingSpeedRange = 20.f;

	//蹲下散射倍率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Multiplier")
	float SpreadAngleMultiplier_Crouching = 1.f;

	//蹲下的散射平滑过渡率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Multiplier")
	float TransitionRate_Crouching = 5.f;

	//是否绘制检测
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults|Spread|Multiplier")
	bool bDrawDebug = false;

private:
	//上次开火的时间戳
	double LastFireTime = 0.0;
	//当前的武器热量
	float CurrentHeat = 0.f;
	//当前的散射角
	float CurrentSpreadAngle = 0.f;
	//是否触发了首发精准
	bool bHasFirstShotAccuracy = false;
	//当前的散射倍率
	float CurrentSpreadAngleMultiplier = 1.f;
	//站立的散射倍率
	float StandingStillMultiplier = 1.f;
	//蹲下的散射倍率
	float CrouchingMultiplier = 1.f;

public:
	//支持网络同步
	virtual bool IsSupportedForNetworking() const override { return true; }

	//获取世界指针
	virtual class UWorld* GetWorld() const override final;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	//武器每帧进行的运算
	//武器的散射倍率，热量等
	void WeaponTick(float DeltaSeconds);

	//装备
	void OnEquipped(UObject* InInstigator);
	//卸载
	void OnUnEquipped(UObject* InInstigator);

	//添加散射
	void AddSpread();
	//更新长按开火时连续射击的时间
	//每次射击GAS会记录开火时间
	void UpdateFiringTime();

public:
	//获取武器槽
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	ESimpleWeaponSlot GetWeaponSlot() const { return WeaponSlot; }

	//获取武器槽名称
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FName GetInventorySlotName() const { return InventorySlotName; }

	//获取武器槽位置矫正值
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FTransform GetInventoryRelativeTransform() const { return InventoryRelativeTransform; }

	//获取武器技能
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	TArray<FWeaponBindingAbility> GetBindingAbilities() const { return BindingAbilities; }

	//获取装备动画
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FSimpleWeaponEquippedMontage GetEquippedMontage() const { return EquippedMontage; }

	//获取卸载动画
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	FSimpleWeaponEquippedMontage GetUnequippedMontage() const { return UnEquippedMontage; }

	//获取当前的子弹量
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	int32 GetCurrentCartridge() const { return CurrentCartridge; }

	//获取弹夹容量
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	int32 GetClipSize() const { return ClipSize; }

	//获取每发子弹的弹丸数量
	int32 GetBulletsPerCartridge() const { return BulletsPerCartridge; }

	//获取当前散射角
	float GetCalculatedSpreadAngle() const { return CurrentSpreadAngle; }

	//获取当前散射角倍率
	float GetCalculatedSpreadAngleMultiplier() const
	{
		return bHasFirstShotAccuracy ? 0.f : CurrentSpreadAngleMultiplier;
	}

	//获取首发精准判定
	bool GetHasFirstShotAccuracy() const { return bHasFirstShotAccuracy; }
	//获取散射角指数
	float GetSpreadExponent() const { return SpreadExponent; }

	//获取最大伤害范围
	float GetMaxDamageDistance() const { return MaxDamageRange; }
	//获取子弹检测范围
	float GetBulletRadius() const { return BulletRadius; }
	//获取是否绘制检测
	bool GetCanDrawDebug() const { return bDrawDebug; }

	//设置装备武器的动画参数
	//单一职责原则，动画相关都交给蓝图执行
	UFUNCTION(BlueprintImplementableEvent, Category="Weapon Animation")
	void SetAnimationParamsOnEquipped(UObject* InInstigator);

	//设置卸载武器的动画参数
	//单一职责原则，动画相关都交给蓝图执行
	UFUNCTION(BlueprintImplementableEvent, Category="Weapon Animation")
	void SetAnimationParamsOnUnEquipped(UObject* InInstigator);

	//武器热量限制
	inline float ClampHeat(float NewHeat)
	{
		float MinHeat;
		float MaxHeat;
		ComputeHeatRange(MinHeat, MaxHeat);

		return FMath::Clamp(NewHeat, MinHeat, MaxHeat);
	}

	//更新散射
	bool UpdateSpread(float DeltaSeconds);
	//更新加成
	bool UpdateMultipliers(float DeltaSeconds);

public:
	//设置行为发起者
	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }
	//子弹消耗
	void CartridgeCost(int32 CostCounts);
	//装弹
	void ReloadCartridge(int32 CartridgeCounts);

	//获取发起者
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	UObject* GetInstigator() const { return Instigator; }

	//获取发起角色
	UFUNCTION(BlueprintPure, Category="Weapon Instance")
	ACharacter* GetCharacter() const;

private:
	//武器散射角范围计算
	void ComputeSpreadRange(float& MinSpread, float& MaxSpread);
	//武器热量范围计算
	void ComputeHeatRange(float& MinHeat, float& MaxHeat);

public:
	//武器实例化
	void InitInstance()
	{
		CurrentCartridge = CartridgeCountsOnSpawn;
	}

private:
	//行为发起者
	UPROPERTY(Replicated)
	UObject* Instigator;

private:
	//当前的子弹量
	UPROPERTY(Replicated)
	int32 CurrentCartridge = 0;
};
