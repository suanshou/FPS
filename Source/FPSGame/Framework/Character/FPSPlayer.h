// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/FPSCharacterBase.h"
#include "Interface/SimpleInputInterface.h"
#include "FPSPlayer.generated.h"

//配置文件归到Game
UCLASS(config=Game)
class FPSGAME_API AFPSPlayer : public AFPSCharacterBase, public ISimpleInputInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSPlayer(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//输入泛化接口
	virtual void K2_OnActionInputTag_Implementation(ETriggerEvent InEvent, const FInputActionValue& InValue, FGameplayTag InTag) override;
	
protected:
	// Called to bind functionality to input
	//绑定输入行为
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//注册输入
	virtual void NotifyControllerChanged() override;
};
