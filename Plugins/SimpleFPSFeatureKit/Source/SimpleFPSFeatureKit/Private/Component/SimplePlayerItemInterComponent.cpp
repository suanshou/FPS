// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SimplePlayerItemInterComponent.h"

#include "BlendSpaceAnalysis.h"
#include "Actor/Item/Core/SimpleItemActorBase.h"


// Sets default values for this component's properties
USimplePlayerItemInterComponent::USimplePlayerItemInterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SelectItemActorRange = 200.f;
	SphereCenterOffset = FVector::ZeroVector;

	TriggerInterval = 0.f;
	CheckCollisionType = ECC_EngineTraceChannel1;
}


// Called when the game starts
void USimplePlayerItemInterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimplePlayerItemInterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//服务器同步，保证所见即所得
	if (GetOwner() && GetOwner()->HasLocalNetOwner())
	{
		CheckItemAroundPlayer();

		TriggerInterval -= DeltaTime;
		if (TriggerInterval <= 0.f)
		{
			TriggerInterval = 0.f;
		}
	}
}

void USimplePlayerItemInterComponent::OnSelectingItemTriggerStart_Implementation(
	ASimpleItemActorBase* InSelectingItem, bool bForceInHand)
{
	Super::OnSelectingItemTriggerStart_Implementation(InSelectingItem, bForceInHand);
}

void USimplePlayerItemInterComponent::OnInteractingItemTriggerEnd_Implementation(
	ASimpleItemActorBase* InInteractingItem, bool bIsPutPack)
{
	Super::OnInteractingItemTriggerEnd_Implementation(InInteractingItem, bIsPutPack);
}

void USimplePlayerItemInterComponent::Trigger(const FInputActionValue& Value)
{
	if (TriggerInterval <= 0.f && SelectingItem.IsValid())
	{
		TriggerInterval = 0.5f;

		StartTriggerInteractingItem();
	}
}

void USimplePlayerItemInterComponent::Throw(const FInputActionValue& Value)
{
	if (TriggerInterval <= 0.f && InteractingItem.IsValid())
	{
		TriggerInterval = 0.5f;

		EndTriggerInteractingItem();
	}
}

void USimplePlayerItemInterComponent::CheckItemAroundPlayer()
{
	//检测是否可以交互
	if (!IsStartInteraction())
	{
		SelectingItem = nullptr;
		return;
	}

	UWorld* WorldPtr = GetWorld();
	APlayerController* PlayerControllerPtr = WorldPtr ? WorldPtr->GetFirstPlayerController() : nullptr;
	APawn* PlayerCharacterPtr = PlayerControllerPtr ? PlayerControllerPtr->GetPawn() : nullptr;

	//清空附近物品，每帧做检测和获取
	NearbyItems.Empty();

	SelectingItem = nullptr;

	double MinItemAngle = 360.f;

	if (WorldPtr && PlayerControllerPtr && PlayerCharacterPtr)
	{
		FVector PlayerLocation = PlayerCharacterPtr->GetActorLocation();
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{UEngineTypes::ConvertToObjectType(CheckCollisionType)};
		TArray<AActor*> ActorsToIgnore{PlayerCharacterPtr, Cast<AActor>(InteractingItem)};

		TArray<FHitResult> HitResults;

		if (UKismetSystemLibrary::SphereTraceMultiForObjects(
			WorldPtr,
			PlayerLocation + SphereCenterOffset,
			PlayerLocation + SphereCenterOffset,
			SelectItemActorRange,
			ObjectTypes,
			false,
			ActorsToIgnore,
			DrawDebugTraceMode,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			2.f))
		{
			for (auto& HitResult : HitResults)
			{
				if (ASimpleItemActorBase* HitItemActor = Cast<ASimpleItemActorBase>(HitResult.GetActor()))
				{
					if (HitItemActor->IsStartTrigger(this, false))
					{
						NearbyItems.Emplace(HitItemActor);

						FVector OutLocation;
						FRotator OutRotation;
						PlayerControllerPtr->GetPlayerViewPoint(OutLocation, OutRotation);

						double DotValue = FVector::DotProduct(
							(HitResult.GetActor()->GetActorLocation() - OutLocation).GetSafeNormal(),
							OutRotation.Vector().GetSafeNormal());

						//弧度转角度
						double TmpItemAngle = FMath::RadiansToDegrees(FMath::Acos(DotValue));

						//后续需要重写，小于最小值
						if (TmpItemAngle < MinItemAngle)
						{
							MinItemAngle = TmpItemAngle;
							SelectingItem = HitItemActor;
						}
					}
				}
			}
		}
	}
}
