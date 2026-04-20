// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimpleItemInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USimpleItemInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLEFPSFEATUREKIT_API ISimpleItemInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//执行接口必须使用Execute
	//FPSCharacterBase继承，SimpleItemActorPickable实现
	//FPSCharacterBase继承，SimpleItemActorWeapon实现
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category="Item Interaction")
	USkeletalMeshComponent* GetCharacterMesh();
};
