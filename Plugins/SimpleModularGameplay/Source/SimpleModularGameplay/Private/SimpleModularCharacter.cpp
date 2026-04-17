// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleModularCharacter.h"

#include "Components/GameFrameworkComponentManager.h"


// // Sets default values
// ASimpleModularCharacter::ASimpleModularCharacter()
// {
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
// }

ASimpleModularCharacter::ASimpleModularCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASimpleModularCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ASimpleModularCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	
	Super::EndPlay(EndPlayReason);
}

// Called when the game starts or when spawned
void ASimpleModularCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
		this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

// Called every frame
void ASimpleModularCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASimpleModularCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
