// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Core/SimpleItemActorBase.h"
#include "SimpleItemActorPickable.generated.h"

class USimpleItemPickableDefinition;
enum class ESimpleKitItemPickableType : uint8;

UCLASS(Abstract)
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorPickable : public ASimpleItemActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleItemActorPickable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//可拾取物品类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ItemActorPickable")
	ESimpleKitItemPickableType PickableType;

	//物品定义
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ItemActorPickable")
	TSubclassOf<USimpleItemPickableDefinition> ItemDefinition;

public:
	//获取可拾取物品类型
	UFUNCTION(BlueprintPure, Category="TPItemActor_Pickable")
	ESimpleKitItemPickableType GetPickableType() const { return PickableType; }

	//获取物品定义
	UFUNCTION(BlueprintPure, Category="TPItemActor_Pickable")
	USimpleItemPickableDefinition* GetItemDefinition() const;

protected:
	//开始激活
	virtual void
	OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand) override;
	//结束激活
	virtual void
	OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack) override;

protected:
	//拾取物品
	//传递的参数是交互组件和模型
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickable")
	void PickUpItem(USimpleItemInterComponent* ItemInterComponent, UPrimitiveComponent* InItemMesh);

	//丢弃物品
	//传递的参数是交互组件，模型和是否放入背包
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickable")
	void ThrowItem(USimpleItemInterComponent* ItemInterComponent, UPrimitiveComponent* InItemMesh, bool bIsPutPack);
};
