// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleFPSInputComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputComponent;
struct FInputActionValue;

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class SIMPLEFPSFEATUREKIT_API USimpleFPSInputComponent : public UActorComponent
{
	//需要对构造函数做特殊处理的导出宏
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this component's properties
	//在开始游戏时将输入动态注册到FPSPlayer
	//USimpleFPSInputComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

public:
	//绑定输入行为
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	//注册输入对照表
	void RegisterMappingContext();

public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
};
