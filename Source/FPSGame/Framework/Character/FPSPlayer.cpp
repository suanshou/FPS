// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayer.h"

#include "Component/SimpleFPSInputComponent.h"
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
                                                    FGameplayTag InTag)
{
	ISimpleInputInterface::K2_OnActionInputTag_Implementation(InEvent, InValue, InTag);
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