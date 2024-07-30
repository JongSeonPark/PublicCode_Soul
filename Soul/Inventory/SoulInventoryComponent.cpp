// Copyright 2024 ChickenGames All rights reserved.


#include "SoulInventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Soul.h"
#include "SoulGameplayStatics.h"
#include "Game/SoulGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Save/SoulSaveGame.h"

USoulInventoryComponent::USoulInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void USoulInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (USoulGameInstance* SoulGameInstance = USoulGameplayStatics::GetSoulGameInstance(GetWorld()))
	{
		SoulGameInstance->OnSoulSave.AddDynamic(this, &USoulInventoryComponent::OnSaveGame);
		if (SoulGameInstance->SoulSaveGame)
		{
			InventoryItems = SoulGameInstance->SoulSaveGame->InventoryItems;
			ItemMap.Empty();
			for (FSoulItemData InventoryItem : InventoryItems)
			{
				ItemMap.Add({InventoryItem.ItemID, InventoryItem});
			}
		}
	}
}

void USoulInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulInventoryComponent, InventoryItems, COND_None, REPNOTIFY_Always);
}

void USoulInventoryComponent::AddItem(FSoulItemData ItemData)
{
	if (ItemData.Amount <= 0) return;

	if (FSoulItemData* FindedSoulItemData = InventoryItems.FindByPredicate([ItemData](const FSoulItemData& Item) { return Item.ItemID == ItemData.ItemID; }))
	{
		FindedSoulItemData->Amount += ItemData.Amount;
		ItemMap[ItemData.ItemID].Amount += ItemData.Amount;
	}
	else
	{
		InventoryItems.Add(ItemData);
		ItemMap.Add({ItemData.ItemID, ItemData});
	}
}

void USoulInventoryComponent::AddItems(const TArray<FSoulItemData>& ItemDatas)
{
	for (FSoulItemData ItemData : ItemDatas)
	{
		AddItem(ItemData);
	}
	OnAddItems_Client(ItemDatas);
}

void USoulInventoryComponent::RemoveItem(FSoulItemData ItemData)
{
	if (ItemData.Amount <= 0) return;

	if (FSoulItemData* FindedSoulItemData = InventoryItems.FindByPredicate([ItemData](const FSoulItemData& Item) { return Item.ItemID == ItemData.ItemID; }))
	{
		FindedSoulItemData->Amount -= ItemData.Amount;
		ItemMap[ItemData.ItemID].Amount -= ItemData.Amount;
		if (FindedSoulItemData->Amount <= 0)
		{
			InventoryItems.RemoveSingle(*FindedSoulItemData);
			ItemMap.Remove(ItemData.ItemID);
		}
		OnRemoveItems_Client({ItemData});
	}
}

void USoulInventoryComponent::ConsumeItem(FName ItemID, int32 Amount)
{
	FSoulItemData* FindedSoulItemData = InventoryItems.FindByPredicate([ItemID](const FSoulItemData& Item) { return Item.ItemID == ItemID; });
	if (!FindedSoulItemData || FindedSoulItemData->Amount < 1)
	{
		SOUL_LOG(Warning, TEXT("%s를 가지고 있지 않습니다."), *ItemID.ToString());
		return;
	}
	FSoulItemDefinition* ItemDefinition = GetItemDefinition(ItemID);
	if (ItemDefinition->Type != ESoulItemType::Consumable || ItemDefinition->ConsumableItemData.IsEmpty())
	{
		SOUL_LOG(Warning, TEXT("%s는 Consumable이 아닙니다."), *ItemDefinition->Name.ToString());
		return;
	}

	FGameplayEventData Payload;
	Payload.Instigator = GetOwner();
	Payload.Target = GetOwner();
	Payload.EventMagnitude = Amount;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), ItemDefinition->ConsumableItemData.AbilityEventTag, Payload);
}

FSoulItemDefinition* USoulInventoryComponent::GetItemDefinition(FName EquipItemID)
{
	if (USoulGameInstance* SoulGameInstance = USoulGameplayStatics::GetSoulGameInstance(GetWorld()))
	{
		FString ContextString;
		return SoulGameInstance->ItemDefinitions->FindRow<FSoulItemDefinition>(EquipItemID, ContextString, false);
	}
	return nullptr;
}

void USoulInventoryComponent::OnRep_InventoryItems(const TArray<FSoulItemData>& Old_InventoryItem)
{
	// SOUL_LOG(Display, TEXT("OLD:"));
	// for (FSoulItemData InventoryItem : Old_InventoryItem)
	// {
	// 	SOUL_LOG(Display, TEXT("%s: Count: %d"), *InventoryItem.ItemID.ToString(), InventoryItem.Amount);
	// }
	// SOUL_LOG(Display, TEXT("==================="));
	// SOUL_LOG(Display, TEXT("New:"));
	// for (FSoulItemData InventoryItem : InventoryItems)
	// {
	// 	SOUL_LOG(Display, TEXT("%s: Count: %d"), *InventoryItem.ItemID.ToString(), InventoryItem.Amount);
	// }
	ItemMap.Empty();
	for (FSoulItemData InventoryItem : InventoryItems)
	{
		ItemMap.Add({InventoryItem.ItemID, InventoryItem});
	}
}

void USoulInventoryComponent::OnSaveGame(USoulSaveGame* SaveGame)
{
	SaveGame->InventoryItems = InventoryItems;
}

void USoulInventoryComponent::OnAddItems_Client_Implementation(const TArray<FSoulItemData>& ItemDatas)
{
	OnAddItemDelegate.Broadcast(ItemDatas);
	// for (FSoulItemData InventoryItem : ItemDatas)
	// {
	// 	SOUL_LOG(Display, TEXT("%s: Count: %d"), *InventoryItem.ItemID.ToString(), InventoryItem.Amount);
	// }
}

void USoulInventoryComponent::OnRemoveItems_Client_Implementation(const TArray<FSoulItemData>& ItemDatas)
{
	OnRemoveItemDelegate.Broadcast(ItemDatas);
}
