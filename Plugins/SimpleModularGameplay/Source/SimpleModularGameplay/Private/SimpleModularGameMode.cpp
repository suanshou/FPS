// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleModularGameMode.h"

#include "SimpleModularGameState.h"
#include "SimpleModularPawn.h"
#include "SimpleModularPlayerController.h"
#include "SimpleModularPlayerState.h"

ASimpleModularGameMode::ASimpleModularGameMode(const FObjectInitializer& ObjectInitializer)
{
	GameStateClass = ASimpleModularGameState::StaticClass();
	PlayerControllerClass = ASimpleModularPlayerController::StaticClass();
	PlayerStateClass = ASimpleModularPlayerState::StaticClass();
	DefaultPawnClass = ASimpleModularPawn::StaticClass();
}

ASimpleModularGameModeBase::ASimpleModularGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	GameStateClass = ASimpleModularGameStateBase::StaticClass();
	PlayerControllerClass = ASimpleModularPlayerController::StaticClass();
	PlayerStateClass = ASimpleModularPlayerState::StaticClass();
	DefaultPawnClass = ASimpleModularPawn::StaticClass();
}
