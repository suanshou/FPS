// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleItemInterComponent.generated.h"

class ASimpleItemActorBase;

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleItemInterComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this component's properties
	//USimpleItemInterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	//在检测范围内的物品
	UPROPERTY(BlueprintReadOnly, Category="Config")
	TWeakObjectPtr<ASimpleItemActorBase> SelectingItem;

	//正在交互的物品
	UPROPERTY(ReplicatedUsing="OnRep_InteractingItem", BlueprintReadOnly, Category="Config")
	TWeakObjectPtr<ASimpleItemActorBase> InteractingItem;

public:
	//获取交互物品
	UFUNCTION(BlueprintCallable, Category="ItemInteractionComponent")
	ASimpleItemActorBase* GetInteractingItem() const;

	//交互入口，保证服务器有值
	UFUNCTION(BlueprintCallable, Category="ItemInteractionComponent")
	void StartTriggerInteractingItem();

	//交互入口，保证服务器有值
	UFUNCTION(BlueprintCallable, Category="ItemInteractionComponent")
	void EndTriggerInteractingItem();

public:
	//是否在交互范围内
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsStartInteraction() const;

	//是否在交互范围内
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsEndInteraction() const;

	//是否可以交互
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsStartTriggerInteractingItem() const;

	//是否可以交互
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="ItemInteractionComponent")
	bool IsEndTriggerInteractingItem() const;

protected:
	//在服务器执行，UDP
	//将检测范围内的物品转换为交互物品
	UFUNCTION(Server, Unreliable)
	void StartTriggerSelectingItemOnServer(ASimpleItemActorBase* InSelectingItem);

	//在服务器执行，UDP
	//丢弃物品或放回背包
	UFUNCTION(Server, Unreliable)
	void EndTriggerSelectingItemOnServer(bool bIsPutPack);

	//蓝图和C++都可以重写
	//服务器StartTrigger的具体执行逻辑
	UFUNCTION(BlueprintNativeEvent, Category="ItemInteractionComponent")
	void OnSelectingItemTriggerStart(ASimpleItemActorBase* InSelectingItem, bool bForceInHand);

	//蓝图和C++都可以重写
	//服务器EndTrigger的具体执行逻辑
	UFUNCTION(BlueprintNativeEvent, Category="ItemInteractionComponent")
	void OnInteractingItemTriggerEnd(ASimpleItemActorBase* InInteractingItem, bool bIsPutPack);

public:
	//泛化外部接口
	void ServerTriggerItem(ASimpleItemActorBase* NewTriggerItem, bool bForceInHand = false);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//拾取时同步更新UI
	UFUNCTION()
	virtual void OnRep_InteractingItem();
};
