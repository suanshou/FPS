// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/Pickable/Core/SimpleItemActorPickable.h"
#include "SimpleItemActorWeapon.generated.h"

class USimpleWeaponInstance;

UCLASS()
class SIMPLEFPSFEATUREKIT_API ASimpleItemActorWeapon : public ASimpleItemActorPickable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleItemActorWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//开始激活
	virtual void
	OnStartTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bForceInHand) override;
	//结束激活
	virtual void
	OnEndTrigger_Implementation(USimpleItemInterComponent* ItemInterComponent, bool bIsPutPack) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//武器模型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	//武器实例管理类
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	TSubclassOf<USimpleWeaponInstance> WeaponInstanceClass;

public:
	//变量网络同步
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//获取武器模型
	UFUNCTION(BlueprintPure, Category="Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	//获取武器实例
	UFUNCTION(BlueprintPure, Category="Weapon")
	USimpleWeaponInstance* GetWeaponInstance() const { return WeaponInstance; }

	//装备到武器槽
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipToSlot();

	//装备到手上
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipToHand();

private:
	//武器实例
	UPROPERTY(Replicated)
	TObjectPtr<USimpleWeaponInstance> WeaponInstance;
};
