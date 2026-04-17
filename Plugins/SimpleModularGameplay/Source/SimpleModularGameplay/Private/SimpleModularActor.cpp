// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleModularActor.h"

#include "Components/GameFrameworkComponentManager.h"


// Sets default values
// ASimpleModularActor::ASimpleModularActor()
// {
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
// }

ASimpleModularActor::ASimpleModularActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASimpleModularActor::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ASimpleModularActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

// Called when the game starts or when spawned
void ASimpleModularActor::BeginPlay()
{
	Super::BeginPlay();

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
		this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

// Called every frame
void ASimpleModularActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
