// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SimpleModularGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SIMPLEMODULARGAMEPLAY_API ASimpleModularGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASimpleModularGameMode(const FObjectInitializer& ObjectInitializer);
};

UCLASS(Blueprintable)
class SIMPLEMODULARGAMEPLAY_API ASimpleModularGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASimpleModularGameModeBase(const FObjectInitializer& ObjectInitializer);
};
