// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SimpleInputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Config/SimpleActionInputConfig.h"
#include "GameFramework/Character.h"
#include "Interface/SimpleInputInterface.h"


//class UEnhancedInputLocalPlayerSubsystem;
// Sets default values for this component's properties
// USimpleInputComponent::USimpleInputComponent()
// {
// 	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
// 	// off to improve performance if you don't need them.
// 	PrimaryComponentTick.bCanEverTick = true;
//
// 	// ...
// }


USimpleInputComponent::USimpleInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void USimpleInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USimpleInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USimpleInputComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputConfig)
		{
			if (ISimpleInputInterface* OwnerInterface = Cast<ISimpleInputInterface>(GetOwner()))
			{
				auto InFunc = [OwnerInterface,this](
					const FInputActionValue& Value,
					ETriggerEvent InEvent,
					FGameplayTag InGamePlayTag)
				{
					ISimpleInputInterface::Execute_K2_OnActionInputTag(GetOwner(), InEvent, Value, InGamePlayTag);
				};

				auto InBindFunc = [&](
					ETriggerEvent InEvent,
					UInputAction* InAction,
					FGameplayTag InGamePlayTag)
				{
					EnhancedInputComponent->BindActionValueLambda(InAction, InEvent, InFunc, InEvent, InGamePlayTag);
				};

				for (const auto& TmpAction : InputConfig->InputActions)
				{
					InBindFunc(ETriggerEvent::Triggered, TmpAction.InputAction, TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Started, TmpAction.InputAction, TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Ongoing, TmpAction.InputAction, TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Canceled, TmpAction.InputAction, TmpAction.InputTag);
					InBindFunc(ETriggerEvent::Completed, TmpAction.InputAction, TmpAction.InputTag);
				}
			}
		}
	}
}

void USimpleInputComponent::RegisterMappingContext()
{
	if (ACharacter* InCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(InCharacter->Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem
				<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}
