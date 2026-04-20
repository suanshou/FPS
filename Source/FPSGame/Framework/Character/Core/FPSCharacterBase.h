// Fill out your copyright notice in the Description page of Project Settings.
//伤害和攻击

#pragma once

#include "CoreMinimal.h"
#include "SimpleModularCharacter.h"
#include "Interface/SimpleItemInteractionInterface.h"
#include "FPSCharacterBase.generated.h"

//伤害和接收伤害
//攻击
//配置文件归到Game
UCLASS(config=Game)
class FPSGAME_API AFPSCharacterBase : public ASimpleModularCharacter, public ISimpleItemInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual USkeletalMeshComponent* GetCharacterMesh_Implementation() override;
};
