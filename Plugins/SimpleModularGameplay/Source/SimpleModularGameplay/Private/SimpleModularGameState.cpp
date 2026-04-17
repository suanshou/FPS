// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleModularGameState.h"

#include "Components/GameFrameworkComponentManager.h"

void ASimpleModularGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ASimpleModularGameState::BeginPlay()
{
	Super::BeginPlay();

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
		this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

void ASimpleModularGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

void ASimpleModularGameStateBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ASimpleModularGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
		this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

void ASimpleModularGameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	
	Super::EndPlay(EndPlayReason);
}
