// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleItemDefinition.h"
#include "SimpleItemPickableDefinition.generated.h"

/**
 * 
 */

class ASimpleItemActorInventory;

UCLASS()
class SIMPLEFPSFEATUREKIT_API USimpleItemPickableDefinition : public USimpleItemDefinition
{
	GENERATED_BODY()

public:
	USimpleItemPickableDefinition();

	//扔出物品的速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defaults")
	float ThrowItemSpeed;

	//最大容量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 ItemMaxCounts;

	//允许拾取后吸附到手上
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	bool bAllowInHand;

	//物品分类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TSubclassOf<ASimpleItemActorInventory> ItemClass;
};
