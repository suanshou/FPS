// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/SimpleActionInputConfig.h"

const UInputAction* USimpleActionInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FSimpleInputAction& TmpAction : InputActions)
	{
		if (TmpAction.InputAction && TmpAction.InputTag == InputTag)
		{
			return TmpAction.InputAction;
		}
	}
	
	return nullptr;
}
