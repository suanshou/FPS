// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SimpleActionInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct SIMPLEINPUTEXPAND_API FSimpleInputAction
{
	GENERATED_BODY()

public:
	//输入行为
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;

	//输入行为对应的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;
};

/**
 * 
 */
UCLASS(BlueprintType, Const)
class SIMPLEINPUTEXPAND_API USimpleActionInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	//找到标签对应的输入行为
	UFUNCTION(BlueprintCallable, Category="InputConfig")
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

public:
	//输入的标签和对应的输入行为
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="InputConfig")
	TArray<FSimpleInputAction> InputActions;
};
