// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/FPSCharacterBase.h"
#include "FPSPlayer.generated.h"

//配置文件归到Game
UCLASS(config=Game)
class FPSGAME_API AFPSPlayer : public AFPSCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//绑定输入行为
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//注册输入
	virtual void NotifyControllerChanged() override;
	
};
