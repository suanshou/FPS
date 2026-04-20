// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/Pickable/Core/SimpleItemActorPickable.h"

#include "SimpleFPSFeatureKitType.h"
#include "Actor/ItemDefinition/SimpleItemPickableDefinition.h"
#include "Component/SimpleItemInterComponent.h"
#include "GameFramework/Character.h"
#include "Interface/SimpleItemInteractionInterface.h"


// Sets default values
ASimpleItemActorPickable::ASimpleItemActorPickable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemType = ESimpleKitItemType::ITEM_PICKABLE;
	PickableType = ESimpleKitItemPickableType::ITEM_NORMAL;
}

// Called when the game starts or when spawned
void ASimpleItemActorPickable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASimpleItemActorPickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

USimpleItemPickableDefinition* ASimpleItemActorPickable::GetItemDefinition() const
{
	return ItemDefinition.GetDefaultObject();
}

void ASimpleItemActorPickable::OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                             bool bForceInHand)
{
	Super::OnStartTrigger_Implementation(ItemInterComponent, bForceInHand);

	USkeletalMeshComponent* CharacterMesh = nullptr;
	if (ISimpleItemInteractionInterface* ItemInterface = Cast<ISimpleItemInteractionInterface>(
		ItemInterComponent->GetOwner()))
	{
		//Execute智能分发给蓝图和C++执行
		CharacterMesh = ItemInterface->Execute_GetCharacterMesh(ItemInterComponent->GetOwner());
	}
	else
	{
		ACharacter* CharacterOwner = Cast<ACharacter>(ItemInterComponent->GetOwner());
		if (CharacterOwner)
		{
			CharacterMesh = CharacterOwner->GetMesh();
		}
	}

	if (CharacterMesh)
	{
		if (ItemDefinition)
		{
			if (ItemDefinition.GetDefaultObject()->StartTriggerAnimMontage)
			{
				//播放动画
				ItemInterComponent->PlayMontageNetMultiCast(ItemDefinition.GetDefaultObject()->StartTriggerAnimMontage);
			}

			//吸附到插槽
			//服务器执行Attach后客户端自动同步，无需广播
			AttachToComponent(
				CharacterMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				ItemDefinition.GetDefaultObject()->ItemSlotName);

			SetActorRelativeTransform(ItemDefinition.GetDefaultObject()->ItemRelativeTransform);
		}
	}
}

void ASimpleItemActorPickable::OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                           bool bIsPutPack)
{
	Super::OnEndTrigger_Implementation(ItemInterComponent, bIsPutPack);
}

void ASimpleItemActorPickable::PickUpItem_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                         UPrimitiveComponent* InItemMesh)
{
	if (InItemMesh)
	{
		//取消物理模拟
		InItemMesh->SetSimulatePhysics(false);
		//插槽相对位置清零
		InItemMesh->SetRelativeLocation(FVector::ZeroVector);
		//取消碰撞检测
		InItemMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

		//关闭运动同步
		//SetReplicatingMovement(false);
	}
}

void ASimpleItemActorPickable::ThrowItem_Implementation(USimpleItemInterComponent* ItemInterComponent,
                                                        UPrimitiveComponent* InItemMesh, bool bIsPutPack)
{
	if (InItemMesh && ItemDefinition)
	{
		//开启运动同步
		//SetReplicatingMovement(true);

		//开启碰撞检测
		InItemMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		//开启物理模拟
		InItemMesh->SetSimulatePhysics(true);

		if (!bIsPutPack)
		{
			if (ItemInterComponent && ItemInterComponent->GetOwner())
			{
				//丢出物品
				InItemMesh->AddImpulse(ItemInterComponent->GetOwner()->GetActorForwardVector() *
				                       ItemDefinition.GetDefaultObject()->ThrowItemSpeed,
				                       NAME_None, true);
			}
		}
	}
}
