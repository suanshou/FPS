// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleInputComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class USimpleActionInputConfig;
class UInputComponent;

UCLASS(BlueprintType, Blueprintable, Meta=(BlueprintSpawnableComponent))
class SIMPLEINPUTEXPAND_API USimpleInputComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this component's properties
	//USimpleInputComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Config|Input")
	TObjectPtr<USimpleActionInputConfig> InputConfig;

public:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	void RegisterMappingContext();
};
