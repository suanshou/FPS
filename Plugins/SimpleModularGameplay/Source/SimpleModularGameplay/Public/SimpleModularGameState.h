// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SimpleModularGameState.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEMODULARGAMEPLAY_API ASimpleModularGameState : public AGameState
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

UCLASS()
class SIMPLEMODULARGAMEPLAY_API ASimpleModularGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
