// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "SimpleInputInterface.generated.h"

enum class ETriggerEvent : uint8;
struct FInputActionValue;

// This class does not need to be modified.
UINTERFACE()
class USimpleInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLEINPUTEXPAND_API ISimpleInputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//按键响应后通知
	//蓝图和C++都可以重写
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Input",meta=(DisplayName="OnActionInputTag"))
	void K2_OnActionInputTag(ETriggerEvent InEvent,const FInputActionValue& InValue,FGameplayTag InTag);
};
