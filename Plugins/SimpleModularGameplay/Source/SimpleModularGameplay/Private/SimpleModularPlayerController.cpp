// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleModularPlayerController.h"

#include "Components/GameFrameworkComponentManager.h"

void ASimpleModularPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ASimpleModularPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
		this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

void ASimpleModularPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	
	Super::EndPlay(EndPlayReason);
}
