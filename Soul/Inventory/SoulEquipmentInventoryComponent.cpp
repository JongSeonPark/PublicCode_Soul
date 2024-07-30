// Copyright 2024 ChickenGames All rights reserved.


#include "SoulEquipmentInventoryComponent.h"

#include "Soul.h"
#include "SoulGameplayStatics.h"
#include "AbilitySystem/SoulAbilitySystemBlueprintLibrary.h"
#include "Game/SoulGameInstance.h"
#include "Item/Equipments/SoulEquipment.h"
#include "Save/SoulSaveGame.h"

void USoulEquipmentInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (USoulGameInstance* SoulGameInstance = USoulGameplayStatics::GetSoulGameInstance(GetWorld()))
	{
		if (SoulGameInstance->SoulSaveGame)
		{
			for (TTuple<ESoulEquipmentSlotType, FEquipmentInventoryData> EquippedItem : SoulGameInstance->SoulSaveGame->EquippedItems)
			{
				EquipItem(EquippedItem.Value.ItemID);
			}
		}
	}
}

void USoulEquipmentInventoryComponent::EquipItem(FName EquipItemID)
{
	EquipItem_Server(EquipItemID);
}

void USoulEquipmentInventoryComponent::UnequipItem(ESoulEquipmentSlotType ItemSlot)
{
	UnequipItem_Server(ItemSlot);
}

void USoulEquipmentInventoryComponent::OnSaveGame(USoulSaveGame* SaveGame)
{
	Super::OnSaveGame(SaveGame);
	SaveGame->EquippedItems = EquippedItems;
}

void USoulEquipmentInventoryComponent::EquipItem_Server_Implementation(FName EquipItemID)
{
	FSoulItemDefinition* ItemDefinition = GetItemDefinition(EquipItemID);
	if (!ItemDefinition || ItemDefinition->EquipmentItemData.IsEmpty()) return;

	const ESoulEquipmentSlotType ItemSlot = ItemDefinition->EquipmentItemData.SoulEquipmentSlotType;

	// 같은 아이템을 호출 했을때, 장착을 해제하고 종료
	if (EquippedItems.Contains(ItemSlot) && EquippedItems[ItemSlot].Equipment->GetClass() == ItemDefinition->EquipmentItemData.EquipmentClass)
	{
		UnequipItem(ItemSlot);
		return;
	}

	UnequipItem(ItemSlot);
	EquipItem_Multicast(EquipItemID);

	if (USoulAbilitySystemComponent* AbilitySystemComponent = USoulAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		if (EquippedItems.Contains(ItemSlot))
			EquippedItems[ItemSlot].Equipment->Equip(AbilitySystemComponent);
	}
}

void USoulEquipmentInventoryComponent::EquipItem_Multicast_Implementation(FName EquipItemID)
{
	UWorld* World = GetWorld();
	FSoulItemDefinition* ItemDefinition = GetItemDefinition(EquipItemID);
	if (ItemDefinition && !ItemDefinition->EquipmentItemData.IsEmpty())
	{
		const ESoulEquipmentSlotType ItemSlot = ItemDefinition->EquipmentItemData.SoulEquipmentSlotType;

		ASoulEquipment* Item = World->SpawnActorDeferred<ASoulEquipment>(ItemDefinition->EquipmentItemData.EquipmentClass,
		                                                                 FTransform::Identity,
		                                                                 GetOwner(),
		                                                                 Cast<APawn>(GetOwner()),
		                                                                 ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		                                                                 ESpawnActorScaleMethod::SelectDefaultAtRuntime);
		Item->SetEquipmentSlotType(ItemSlot);
		EquippedItems.Add({ItemSlot, FEquipmentInventoryData(Item, EquipItemID)});
		EquipmentChangeDelegate.Broadcast(ItemSlot);
		Item->FinishSpawning(FTransform::Identity);
	}
}

void USoulEquipmentInventoryComponent::UnequipItem_Server_Implementation(ESoulEquipmentSlotType EquipmentSlotType)
{
	if (EquippedItems.Contains(EquipmentSlotType))
	{
		EquippedItems[EquipmentSlotType].Equipment->Unequip();
	}
	UnequipItem_Multicast(EquipmentSlotType);
}

void USoulEquipmentInventoryComponent::UnequipItem_Multicast_Implementation(ESoulEquipmentSlotType EquipmentSlotType)
{
	if (EquippedItems.Contains(EquipmentSlotType))
	{
		ASoulEquipment* Item = EquippedItems[EquipmentSlotType].Equipment;
		Item->Destroy();
		EquippedItems.Remove(EquipmentSlotType);
		EquipmentChangeDelegate.Broadcast(EquipmentSlotType);
	}
}
