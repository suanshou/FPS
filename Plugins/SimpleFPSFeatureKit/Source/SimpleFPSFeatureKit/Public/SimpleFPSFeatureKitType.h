#pragma once

#include "CoreMinimal.h"
#include "SimpleFPSFeatureKitType.generated.h"

UENUM(BlueprintType)
enum class ESimpleKitItemType : uint8
{
	ITEM_PICKABLE UMETA(DisplayName = "Pickable"),
	ITEM_SCENEINTERACTIVE UMETA(DisplayName = "Scene Interactive"),
	ITEM_INVALID UMETA(DisplayName = "Invalid", Hidden),
};
