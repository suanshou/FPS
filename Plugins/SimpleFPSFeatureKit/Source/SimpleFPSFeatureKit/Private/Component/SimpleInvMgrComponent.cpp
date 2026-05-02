// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SimpleInvMgrComponent.h"

#include "Actor/Item/Pickable/Inventory/SimpleItemActorInventory.h"
#include "Component/SimplePlayerItemInterComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USimpleInvMgrComponent::USimpleInvMgrComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//支持网络同步
	SetIsReplicatedByDefault(true);
}


void FSimpleItemInventoryList::SetInventorySize(const int32& NewInventorySize)
{
	//如果容量小于物品数报错
	if (NewInventorySize < Entries.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("[FSimpleItemInventoryList::SetInventorySize]无效容量值！"));
		return;
	}

	InventorySize = NewInventorySize;
}

bool FSimpleItemInventoryList::IsAddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef,
                                          FSimpleItemInventoryEntry*& ItemEntry)
{
	for (auto& TmpEntry : Entries)
	{
		//找到对应的物品分类
		if (TmpEntry.ItemDefinition == NewItemDef)
		{
			//背包内物品数小于最大容量
			if (TmpEntry.ItemCounts < NewItemDef.GetDefaultObject()->ItemMaxCounts)
			{
				ItemEntry = &TmpEntry;
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[FSimpleItemInventoryList::IsAddEntry]物品数超过了最大容量"));
				return false;
			}
		}
	}

	//当前数量小于容量
	return Entries.Num() < InventorySize;
}

bool FSimpleItemInventoryList::IsRemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef,
                                             const int32& ItemCounts, FSimpleItemInventoryEntry*& ItemEntry)
{
	for (auto& TmpEntry : Entries)
	{
		if (TmpEntry.ItemDefinition == InItemDef)
		{
			if (TmpEntry.ItemCounts < ItemCounts)
			{
				UE_LOG(LogTemp, Error, TEXT("[FSimpleItemInventoryList::IsRemoveEntry]移除物品数量超过背包内物品数量不能移除物品"));
				return false;
			}
			else
			{
				ItemEntry = &TmpEntry;
				return true;
			}
		}
	}

	return false;
}

int32 FSimpleItemInventoryList::AddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef,
                                         const int32& ItemCounts)
{
	//真正要添加的数量
	int32 RealAddCounts = 0;
	//目标物品
	FSimpleItemInventoryEntry* TargetItemEntry = nullptr;

	if (IsAddEntry(NewItemDef, TargetItemEntry))
	{
		//上一个物品数量
		int32 LastCounts = 0;

		//是否存在目标物品
		if (TargetItemEntry)
		{
			//添加到库存
			LastCounts = TargetItemEntry->ItemCounts;
			TargetItemEntry->ItemCounts += ItemCounts;
		}
		else
		{
			//创建目标物品
			TargetItemEntry = &Entries.AddDefaulted_GetRef();
			TargetItemEntry->ItemDefinition = NewItemDef;
			TargetItemEntry->ItemCounts = ItemCounts;
		}

		//限制目标物品数量
		if (TargetItemEntry->ItemCounts > NewItemDef.GetDefaultObject()->ItemMaxCounts)
		{
			TargetItemEntry->ItemCounts = NewItemDef.GetDefaultObject()->ItemMaxCounts;
		}

		RealAddCounts = TargetItemEntry->ItemCounts - LastCounts;

		//标记为脏
		//只同步更改的物品
		MarkItemDirty(*TargetItemEntry);
	}

	return RealAddCounts;
}

int32 FSimpleItemInventoryList::RemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef,
                                            const int32& ItemCounts)
{
	//真正要添加的数量
	int32 RealRemoveCounts = 0;
	//目标物品
	FSimpleItemInventoryEntry* TargetInventoryEntry = nullptr;

	if (IsRemoveEntry(InItemDef, ItemCounts, TargetInventoryEntry))
	{
		TargetInventoryEntry->ItemCounts -= ItemCounts;

		//是否背包内物品堆叠数量为0
		if (TargetInventoryEntry->ItemCounts == 0)
		{
			//移除数组元素
			Entries.RemoveSingle(*TargetInventoryEntry);

			//标记为脏
			MarkArrayDirty();
		}
		else
		{
			MarkItemDirty(*TargetInventoryEntry);
		}

		RealRemoveCounts = ItemCounts;
	}

	return RealRemoveCounts;
}

FSimpleItemInventoryEntry* FSimpleItemInventoryList::GetEntry(
	const TSubclassOf<USimpleItemPickableDefinition>& InItemDef)
{
	//遍历需要获取的物品，相当于循环查找
	return Entries.FindByPredicate([InItemDef](const FSimpleItemInventoryEntry& TmpEntry)-> bool
	{
		return TmpEntry.ItemDefinition == InItemDef;
	});
}

// Called when the game starts
void USimpleInvMgrComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleInvMgrComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USimpleInvMgrComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

USimplePlayerItemInterComponent* USimpleInvMgrComponent::GetItemInteractionComponent()
{
	check(GetOwner());

	return GetOwner()->FindComponentByClass<USimplePlayerItemInterComponent>();
}

bool USimpleInvMgrComponent::GetItemEntry(TSubclassOf<USimpleItemPickableDefinition> ItemDef,
                                          FSimpleItemInventoryEntry& TargetEntry)
{
	//默认初始化
	TargetEntry = FSimpleItemInventoryEntry();

	//获取对应物品
	if (FSimpleItemInventoryEntry* ItemEntry = InventoryList.GetEntry(ItemDef))
	{
		TargetEntry = *ItemEntry;
		return true;
	}

	return false;
}

bool USimpleInvMgrComponent::IsAddItemToInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef)
{
	FSimpleItemInventoryEntry* TempEntry = nullptr;
	return InventoryList.IsAddEntry(ItemDef, TempEntry);
}

bool USimpleInvMgrComponent::IsRemoveItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef,
                                                       const int32& ItemCounts)
{
	FSimpleItemInventoryEntry* TempEntry = nullptr;
	return InventoryList.IsRemoveEntry(ItemDef, ItemCounts, TempEntry);
}

int32 USimpleInvMgrComponent::AddItemToInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef,
                                                 const int32& ItemCounts)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	return InventoryList.AddEntry(ItemDef, ItemCounts);
}

int32 USimpleInvMgrComponent::RemoveItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef,
                                                      const int32& ItemCounts)
{
	check(GetOwner() && GetOwner()->HasAuthority());

	return InventoryList.RemoveEntry(ItemDef, ItemCounts);
}

void USimpleInvMgrComponent::DiscardItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef,
                                                      const int32& ItemCounts)
{
}

void USimpleInvMgrComponent::TakeOutItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef)
{
	check(GetOwner()&& GetOwner()->HasAuthority() && GetWorld());

	if (ItemDef)
	{
		USimplePlayerItemInterComponent* IC_Player = GetItemInteractionComponent();
		const USimpleItemPickableDefinition* TmpItemDef = ItemDef.GetDefaultObject();

		if (IC_Player && TmpItemDef->bAllowInHand && TmpItemDef->ItemClass)
		{
			if (InventoryList.RemoveEntry(ItemDef, 1))
			{
			}
		}
	}
}

void USimpleInvMgrComponent::DiscardItemFromInventoryOnServer_Implementation(
	TSubclassOf<USimpleItemPickableDefinition> ItemDef, int32 ItemCounts)
{
	DiscardItemFromInventory(ItemDef, ItemCounts);
}

void USimpleInvMgrComponent::TakeOutItemFromInventoryOnServer_Implementation(
	TSubclassOf<USimpleItemPickableDefinition> ItemDef)
{
	TakeOutItemFromInventory(ItemDef);
}
