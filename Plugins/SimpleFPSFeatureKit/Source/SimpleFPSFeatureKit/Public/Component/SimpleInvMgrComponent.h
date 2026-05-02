// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ItemDefinition/SimpleItemPickableDefinition.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SimpleInvMgrComponent.generated.h"

class USimplePlayerItemInterComponent;
//库存物品
USTRUCT(BlueprintType)
struct FSimpleItemInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	//物品分类
	UPROPERTY(BlueprintReadOnly, Category="Inventory Entry")
	TSubclassOf<USimpleItemPickableDefinition> ItemDefinition;

	//物品数量
	UPROPERTY(BlueprintReadOnly, Category="Inventory Entry")
	int32 ItemCounts;
};

//==重载
inline bool operator==(const FSimpleItemInventoryEntry& SourceEntry, const FSimpleItemInventoryEntry& TargetEntry)
{
	return SourceEntry.ItemDefinition == TargetEntry.ItemDefinition &&
		SourceEntry.ItemCounts == TargetEntry.ItemCounts;
}

//库存列表类
//第一步，使用FastArray做序列化，有增量同步机制
USTRUCT(BlueprintType)
struct FSimpleItemInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	//以下是库存的增删改查
	//设置库存大小
	void SetInventorySize(const int32& NewInventorySize);
	//是否能添加物品
	bool IsAddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef,
	                FSimpleItemInventoryEntry*& ItemEntry);
	//是否能移除物品
	bool IsRemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef, const int32& ItemCounts,
	                   FSimpleItemInventoryEntry*& ItemEntry);
	//添加物品
	int32 AddEntry(const TSubclassOf<USimpleItemPickableDefinition>& NewItemDef, const int32& ItemCounts);
	//移除物品
	int32 RemoveEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef, const int32& ItemCounts);
	//获取物品
	FSimpleItemInventoryEntry* GetEntry(const TSubclassOf<USimpleItemPickableDefinition>& InItemDef);

public:
	//网络序列化
	//第二步，告诉UE怎么去同步
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSimpleItemInventoryEntry, FSimpleItemInventoryList>(
			Entries, DeltaParams, *this);
	}

public:
	//完成更新UI，生成对象等操作
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
	{
	}

	void PreReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
	{
	}

	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
	{
	}

private:
	//库存物品
	UPROPERTY(BlueprintReadOnly, Category="Inventory List", meta=(AllowPrivateAccess = "true"))
	TArray<FSimpleItemInventoryEntry> Entries;

	//库存大小
	//最大容量
	UPROPERTY(BlueprintReadOnly, Category="Inventory List", meta=(AllowPrivateAccess = "true"))
	int32 InventorySize = 1;
};

//元编程用法
//第三步，告诉UE走增量同步机制
template <>
struct TStructOpsTypeTraits<FSimpleItemInventoryList> : public TStructOpsTypeTraitsBase2<FSimpleItemInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

//库存管理类
UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleInvMgrComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this component's properties
	//USimpleInvMgrComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//获取玩家交互组件
	USimplePlayerItemInterComponent* GetItemInteractionComponent();

public:
	//获取库存列表
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Inventory Manager Component")
	FSimpleItemInventoryList GetInventoryList() { return InventoryList; }

	//获取库存物品实例
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Inventory Manager Component")
	bool GetItemEntry(TSubclassOf<USimpleItemPickableDefinition> ItemDef, FSimpleItemInventoryEntry& TargetEntry);

	//是否可以添加物品
	UFUNCTION(BlueprintCallable, Category = "Inventory Manager Component")
	bool IsAddItemToInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef);

	//是否可以移除物品
	UFUNCTION(BlueprintCallable, Category = "Inventory Manager Component")
	bool IsRemoveItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef, const int32& ItemCounts);

	//添加物品
	UFUNCTION(BlueprintCallable, Category = "Inventory Manager Component")
	int32 AddItemToInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef, const int32& ItemCounts);

	//移除物品
	UFUNCTION(BlueprintCallable, Category = "Inventory Manager Component")
	int32 RemoveItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef, const int32& ItemCounts);

	//丢弃物品
	UFUNCTION(BlueprintCallable, Category = "Inventory Manager Component")
	void DiscardItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef, const int32& ItemCounts);

	//取出物品
	UFUNCTION(BlueprintCallable, Category = "Inventory Manager Component")
	void TakeOutItemFromInventory(const TSubclassOf<USimpleItemPickableDefinition>& ItemDef);

	UFUNCTION(BlueprintCallable, Category = "Inventory Manager Component", Server, Reliable)
	void DiscardItemFromInventoryOnServer(TSubclassOf<USimpleItemPickableDefinition> ItemDef, int32 ItemCounts);

	UFUNCTION(BlueprintCallable, Category = "Inventory Manager Component", Server, Reliable)
	void TakeOutItemFromInventoryOnServer(TSubclassOf<USimpleItemPickableDefinition> ItemDef);

private:
	//库存列表
	UPROPERTY(Replicated)
	FSimpleItemInventoryList InventoryList;
};
