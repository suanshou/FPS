// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimpleItemDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLEFPSFEATUREKIT_API USimpleItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	USimpleItemDefinition();

	//动画对应的插槽名称
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defaults")
	FName ItemSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defaults")
	FTransform ItemRelativeTransform;

	//开始交互动画
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defaults")
	TObjectPtr<UAnimMontage> StartTriggerAnimMontage;

	//结束交互动画
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defaults")
	TObjectPtr<UAnimMontage> EndTriggerAnimMontage;
};
