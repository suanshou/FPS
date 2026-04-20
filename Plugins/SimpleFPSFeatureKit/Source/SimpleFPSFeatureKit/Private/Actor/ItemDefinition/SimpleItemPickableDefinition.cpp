// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ItemDefinition/SimpleItemPickableDefinition.h"

USimpleItemPickableDefinition::USimpleItemPickableDefinition()
{
	ThrowItemSpeed = 10.f;
	ItemMaxCounts = 1;
	bAllowInHand = true;
}
