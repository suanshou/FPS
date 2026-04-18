// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Core/SimpleItemActorBase.h"

#include "SimpleFPSFeatureKitType.h"
#include "Net/UnrealNetwork.h"
#include "Component/SimpleItemInterComponent.h"

// Sets default values
ASimpleItemActorBase::ASimpleItemActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bNetLoadOnClient = false;

	ItemType = ESimpleKitItemType::ITEM_INVALID;
}

// Called when the game starts or when spawned
void ASimpleItemActorBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASimpleItemActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimpleItemActorBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASimpleItemActorBase, InteractingComponent);
}

bool ASimpleItemActorBase::StartTrigger(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand)
{
	if (!IsStartTrigger(ItemInterComponent, bForceInHand))
	{
		UE_LOG(LogTemp, Error, TEXT("[ASimpleItemActorBase::StartTrigger]%s正在开始激活！"), *GetName());
		return false;
	}

	OnStartTrigger(ItemInterComponent, bForceInHand);

	return true;
}

bool ASimpleItemActorBase::EndTrigger(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack)
{
	if (IsEndTrigger(ItemInterComponent, bIsPutPack))
	{
		UE_LOG(LogTemp, Error, TEXT("[ASimpleItemActorBase::EndTrigger]%s正在结束激活！"), *GetName());
	}

	OnEndTrigger(ItemInterComponent, bIsPutPack);

	return true;
}

bool ASimpleItemActorBase::IsStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                         bool bForceInHand)
{
	//没有激活的交互物品，并且传入的交互组件有效
	return InteractingComponent.Get() == nullptr && ItemInterComponent;
}

bool ASimpleItemActorBase::IsEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack)
{
	return InteractingComponent.IsValid() && InteractingComponent.Get() == ItemInterComponent;
}

void ASimpleItemActorBase::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                         bool bForceInHand)
{
	InteractingComponent = ItemInterComponent;
}

void ASimpleItemActorBase::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                       bool bIsPutPack)
{
	InteractingComponent.Reset();
}
