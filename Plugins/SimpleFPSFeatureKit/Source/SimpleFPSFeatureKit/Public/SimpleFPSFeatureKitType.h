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

UENUM(BlueprintType)
enum class ESimpleKitItemPickableType : uint8
{
	ITEM_NORMAL UMETA(DisplayName = "Normal"),
	ITEM_INVENTORY UMETA(DisplayName = "Inventory"),
	ITEM_WEAPON UMETA(DisplayName = "Weapon"),
	ITEM_INVALID UMETA(DisplayName = "Invalid", Hidden),
};

UENUM(BlueprintType)
enum class ESimpleItemInventoryType : uint8
{
	ITEM_NORMAL UMETA(DisplayName = "Normal"),
	ITEM_SUPPLY UMETA(DisplayName = "Supply"),
	ITEM_INVALID UMETA(DisplayName = "Invalid", Hidden),
};

UENUM(BlueprintType)
enum class ESimpleWeaponSlot : uint8
{
	WS_PISTOL = 0 UMETA(DisplayName = "Pistol"),
	WS_RIFLE = 1 UMETA(DisplayName = "Rifle"),
	WS_SMG = 2 UMETA(DisplayName = "Smg"),
	WS_SNIPER = 3 UMETA(DisplayName = "Sniper"),
	WS_INVALID = 255 UMETA(DisplayName = "Invalid", Hidden),
};
