// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Pickable/Core/SimpleItemActorPickable.h"
#include "SimpleItemActorInventory.generated.h"

enum class ESimpleItemInventoryType : uint8;

UCLASS()
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorInventory : public ASimpleItemActorPickable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleItemActorInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//库存物品类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	ESimpleItemInventoryType InventoryType;

public:
	//获取库存物品类型
	UFUNCTION(BlueprintPure, Category = "Inventory")
	ESimpleItemInventoryType GetInventoryType() const { return InventoryType; }

	//设置数量
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetItemCounts(int32 InItemCounts);

	//获取数量
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetItemCounts() const { return ItemCounts; }

	//是否可以触发
	virtual bool
	IsStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand) override;
	//开始触发
	virtual void
	OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand) override;
	//结束触发
	virtual void
	OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack) override;

protected:
	//拾取到背包
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 PickItemToInventory();

protected:
	//物品数量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 ItemCounts;
};
