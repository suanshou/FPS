// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleModularAIController.h"

#include "Components/GameFrameworkComponentManager.h"


// Sets default values
ASimpleModularAIController::ASimpleModularAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASimpleModularAIController::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ASimpleModularAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

// Called when the game starts or when spawned
void ASimpleModularAIController::BeginPlay()
{
	Super::BeginPlay();

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
		this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

// Called every frame
void ASimpleModularAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
