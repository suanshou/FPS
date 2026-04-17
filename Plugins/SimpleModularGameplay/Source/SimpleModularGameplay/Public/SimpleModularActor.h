// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleModularActor.generated.h"

UCLASS()
class SIMPLEMODULARGAMEPLAY_API ASimpleModularActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleModularActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void PreInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
