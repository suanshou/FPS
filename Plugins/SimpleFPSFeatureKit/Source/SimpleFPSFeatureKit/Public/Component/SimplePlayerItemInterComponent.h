// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleItemInterComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SimplePlayerItemInterComponent.generated.h"


struct FInputActionValue;

//玩家交互组件
UCLASS(BlueprintType,Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimplePlayerItemInterComponent : public USimpleItemInterComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimplePlayerItemInterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	//检测玩家周围的物品并更新交互时间间隔
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	//碰撞通道
	UPROPERTY(EditDefaultsOnly, Category="Config")
	TEnumAsByte<ECollisionChannel> CheckCollisionType;

	//Debug类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTraceMode;

	//检测半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Config", meta=(ClampMin=0.f))
	float SelectItemActorRange;

	//检测原点的位置偏移
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	FVector SphereCenterOffset;

public:
	//开始交互
	virtual void
	OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem, bool bForceInHand) override;
	//结束交互
	virtual void
	OnInteractingItemTriggerEnd_Implementation(ASimpleItemActorBase* InInteractingItem, bool bIsPutPack) override;

public:
	//交互
	virtual void Trigger(const FInputActionValue& Value);
	//丢弃
	virtual void Throw(const FInputActionValue& Value);

public:
	//选取检测范围内距离视野最近的物品
	void CheckItemAroundPlayer();

public:
	//玩家周围的物品
	UPROPERTY(BlueprintReadOnly, Category="Config")
	TArray<ASimpleItemActorBase*> NearbyItems;

protected:
	//触发交互的时间间隔
	UPROPERTY()
	float TriggerInterval;
};
