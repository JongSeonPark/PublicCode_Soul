// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SoulItemTypes.generated.h"

class ASoulEquipment;
class USoulGameplayAbility;

UENUM(BlueprintType)
enum class ESoulItemQuality : uint8
{
	Poor,
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary
};

UENUM(BlueprintType)
enum class ESoulItemType : uint8
{
	Miscellaneous,
	Equipment,
	Consumable,
	Currency,
};

UENUM(BlueprintType)
enum class ESoulEquipmentSlotType : uint8
{
	None,
	Head,
	Shoulder,
	Chest,
	Hands,
	Legs,
	Feet,
	Back,
	Waist,
	Accessory,
	Earring,
	Ring,
	Trinket,
	MainHand,
	OffHand,
};

UENUM(BlueprintType)
enum class ESoulEquipmentType : uint8
{
	None,
	Armor,
	Weapon,
	Shield,
	Miscellaneous
};

USTRUCT(BlueprintType)
struct FSoulItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Amount = 0;
	
	bool operator==(const FSoulItemData & other) const
	{
		return (other.ItemID == ItemID) && (other.Amount == Amount);
	}
};

USTRUCT(BlueprintType)
struct FEquipmentItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	ESoulEquipmentSlotType SoulEquipmentSlotType = ESoulEquipmentSlotType::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<ASoulEquipment> EquipmentClass;

	bool IsEmpty() const;
};

USTRUCT(BlueprintType)
struct FConsumableItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FGameplayTag AbilityEventTag;

	bool IsEmpty() const;
};

USTRUCT()
struct FSoulItemDefinition : public FTableRowBase
{
	GENERATED_BODY()

	// ItemID는 TableRow로 대체
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	// FName ItemID = FName();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText Name = FText();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText Description = FText();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	ESoulItemQuality Quality = ESoulItemQuality::Common;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	ESoulItemType Type = ESoulItemType::Miscellaneous;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bIsStackable = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bIsDroppable = false;

	// ESoulItemType가 Equipment일 때 사용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FEquipmentItemData EquipmentItemData;
	// ESoulItemType가 Consumable일 때 사용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FConsumableItemData ConsumableItemData;
};

USTRUCT(BlueprintType)
struct FSoulQualityColors
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TMap<ESoulItemQuality, FLinearColor> QualityColors =
	{
		{ESoulItemQuality::Poor, FLinearColor(0.3f, 0.3f, 0.3f)},
		{ESoulItemQuality::Common, FLinearColor(1.f, 1.f, 1.f)},
		{ESoulItemQuality::Uncommon, FLinearColor(0.12f, 1.f, 0.f)},
		{ESoulItemQuality::Rare, FLinearColor(0.f, 0.44f, 0.87f)},
		{ESoulItemQuality::Epic, FLinearColor(0.64f, 0.21f, 0.93f)},
		{ESoulItemQuality::Legendary, FLinearColor(1.f, 0.5f, 0.f)},
	};
};

USTRUCT(BlueprintType)
struct FEquipmentInventoryData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	ASoulEquipment* Equipment = nullptr;
	UPROPERTY(BlueprintReadOnly)
	FName ItemID;	
};