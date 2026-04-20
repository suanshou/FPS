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

public:
	void InitInstance()
	{
	}

protected:
	//武器类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults | Basic")
	ESimpleWeaponSlot WeaponSlot = ESimpleWeaponSlot::WS_INVALID;

	//武器槽名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults | Basic")
	FName InventorySlotName;

	//插槽相对位置矫正
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults | Basic")
	FTransform InventoryRelativeTransform;

	//武器技能
	//通过按键激活GAS
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Instance Defaults | Basic")
	TArray<FWeaponBindingAbility> BindingAbilities;
};
