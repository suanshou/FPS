// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SimpleItemInterComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USimpleItemInterComponent::USimpleItemInterComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	// ...
	//支持网络同步
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USimpleItemInterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleItemInterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ASimpleItemActorBase* USimpleItemInterComponent::GetInteractingItem() const
{
	return InteractingItem.Get();
}

void USimpleItemInterComponent::StartTriggerInteractingItem()
{
	if (!IsStartTriggerInteractingItem())
	{
		UE_LOG(LogTemp, Error, TEXT("[USimpleItemInterComponent::StartTriggerInteractingItem]不能在本地执行！"));
		return;
	}

	StartTriggerSelectingItemOnServer(SelectingItem.Get());
}

void USimpleItemInterComponent::EndTriggerInteractingItem()
{
	if (!IsEndTriggerInteractingItem())
	{
		UE_LOG(LogTemp, Log, TEXT("[USimpleItemInterComponent::EndTriggerInteractingItem]不能在本地执行！"));
		return;
	}

	EndTriggerSelectingItemOnServer(false);
}

void USimpleItemInterComponent::ServerTriggerItem(ASimpleItemActorBase* NewTriggerItem, bool bForceInHand)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		OnSelectingItemTriggerStart(NewTriggerItem, bForceInHand);
	}
}

void USimpleItemInterComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USimpleItemInterComponent, InteractingItem);
}

void USimpleItemInterComponent::OnRep_InteractingItem()
{
}

void USimpleItemInterComponent::OnSelectingItemTriggerStart_Implementation(ASimpleItemActorBase* InSelectingItem,
                                                                           bool bForceInHand)
{
}

void USimpleItemInterComponent::OnInteractingItemTriggerEnd_Implementation(ASimpleItemActorBase* InInteractingItem,
                                                                           bool bIsPutPack)
{
}

void USimpleItemInterComponent::StartTriggerSelectingItemOnServer_Implementation(ASimpleItemActorBase* InSelectingItem)
{
	OnSelectingItemTriggerStart(InSelectingItem, false);
}

void USimpleItemInterComponent::EndTriggerSelectingItemOnServer_Implementation(bool bIsPutPack)
{
	OnInteractingItemTriggerEnd(InteractingItem.Get(), bIsPutPack);
}

bool USimpleItemInterComponent::IsStartTriggerInteractingItem_Implementation() const
{
	if (!SelectingItem.IsValid())
	{
		UE_LOG(LogTemp, Error,
		       TEXT("[USimpleItemInterComponent::IsStartTriggerInteractingItem_Implementation]检测范围内没有物品！"));
		return false;
	}

	return true;
}

bool USimpleItemInterComponent::IsEndTriggerInteractingItem_Implementation() const
{
	if (InteractingItem == nullptr)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("[USimpleItemInterComponent::IsEndTriggerInteractingItem_Implementation]没有正在交互的物品！"))
		return false;
	}

	return true;
}

bool USimpleItemInterComponent::IsStartInteraction_Implementation() const
{
	return true;
}

bool USimpleItemInterComponent::IsEndInteraction_Implementation() const
{
	return true;
}
