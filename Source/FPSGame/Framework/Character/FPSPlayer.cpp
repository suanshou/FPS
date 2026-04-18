// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayer.h"

#include "EnhancedInputComponent.h"
#include "Component/SimpleFPSInputComponent.h"
#include "Component/SimplePlayerItemInterComponent.h"
#include "Components/SimpleInputComponent.h"


//防止代码在调试过程中被优化
UE_DISABLE_OPTIMIZATION

// Sets default values
// AFPSPlayer::AFPSPlayer()
// {
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
// }

AFPSPlayer::AFPSPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSPlayer::K2_OnActionInputTag_Implementation(ETriggerEvent InEvent, const FInputActionValue& InValue,
                                                    FGameplayTag InputTag)
{
	if (InputTag.GetTagName() == TEXT("PickUp"))
	{
		if (InEvent == ETriggerEvent::Triggered)
		{
			if (InteractionComponent.IsValid())
			{
				InteractionComponent->Trigger(InValue);
			}
		}
	}
	else if (InputTag.GetTagName() == TEXT("Throw"))
	{
		if (InEvent == ETriggerEvent::Triggered)
		{
			if (InteractionComponent.IsValid())
			{
				InteractionComponent->Throw(InValue);
			}
		}
	}
}

// Called to bind functionality to input
void AFPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (USimpleFPSInputComponent* InFPSInputComponent = FindComponentByClass<USimpleFPSInputComponent>())
	{
		InFPSInputComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}

	if (USimpleInputComponent* InSimpleInputComponent = FindComponentByClass<USimpleInputComponent>())
	{
		InSimpleInputComponent->SetupPlayerInputComponent(PlayerInputComponent);
	}

	InteractionComponent = FindComponentByClass<USimplePlayerItemInterComponent>();
}

void AFPSPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (USimpleFPSInputComponent* InFPSInputComponent = FindComponentByClass<USimpleFPSInputComponent>())
	{
		InFPSInputComponent->RegisterMappingContext();
	}

	if (USimpleInputComponent* InSimpleInputComponent = FindComponentByClass<USimpleInputComponent>())
	{
		InSimpleInputComponent->RegisterMappingContext();
	}
}

UE_ENABLE_OPTIMIZATION
