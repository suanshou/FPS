// Fill out your copyright notice in the Description page of Project Settings.
//先列出共性API，再去写代码，防止后续重构

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleItemActorBase.generated.h"

class USimpleItemInterComponent;
enum class ESimpleKitItemType : uint8;

UCLASS()
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleItemActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ItemActorBase")
	ESimpleKitItemType ItemType;

public:
	//开始激活
	UFUNCTION(BlueprintCallable, Category="ItemActorBase")
	bool StartTrigger(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand = false);

	//结束激活
	UFUNCTION(BlueprintCallable, Category="ItemActorBase")
	bool EndTrigger(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack);

	//判断是否可以开始激活
	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	bool IsStartTrigger(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand);

	//判断是否可以结束激活
	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	bool IsEndTrigger(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack);

	//获取交互物品类型
	UFUNCTION(BlueprintPure, Category="ItemActorBase")
	ESimpleKitItemType GetItemType() { return ItemType; }

protected:
	//蓝图和C++都可以重写
	//开始激活
	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	void OnStartTrigger(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand);

	//蓝图和C++都可以重写
	//结束激活
	UFUNCTION(BlueprintNativeEvent, Category="ItemActorBase")
	void OnEndTrigger(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack);


	UPROPERTY(Replicated, BlueprintReadOnly, Category="ItemActorBase")
	TWeakObjectPtr<USimpleItemInterComponent> InteractingComponent;
};
