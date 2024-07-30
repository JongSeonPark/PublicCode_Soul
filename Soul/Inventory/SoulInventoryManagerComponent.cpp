// Copyright 2024 ChickenGames All rights reserved.


#include "SoulInventoryManagerComponent.h"

#include "Soul.h"
#include "SoulGameplayStatics.h"
#include "Character/SoulCharacterBase.h"
#include "Game/SoulGameInstance.h"

void USoulInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	EquipmentInventoryComponent = GetOwner()->GetComponentByClass<USoulEquipmentInventoryComponent>();
}

void USoulInventoryManagerComponent::TryUseItem(FName ItemID)
{
	if (USoulGameInstance* SoulGameInstance = USoulGameplayStatics::GetSoulGameInstance(GetWorld()))
	{
		FString ContextString;
		if (FSoulItemDefinition* ItemDefinition = SoulGameInstance->ItemDefinitions->FindRow<FSoulItemDefinition>(ItemID, ContextString, false))
		{
			switch (ItemDefinition->Type)
			{
			case ESoulItemType::Miscellaneous:
				break;
			case ESoulItemType::Equipment:
				EquipItem(ItemID);
				break;
			case ESoulItemType::Consumable:
				if (EquipmentInventoryComponent)
					EquipmentInventoryComponent->ConsumeItem(ItemID);
				break;
			case ESoulItemType::Currency:
				break;
			}
		}
	}
}

void USoulInventoryManagerComponent::EquipItem(FName EquipItemID)
{
	if (EquipmentInventoryComponent)
		EquipmentInventoryComponent->EquipItem(EquipItemID);
}

void USoulInventoryManagerComponent::UnequipItem(ESoulEquipmentSlotType ItemSlot)
{
	if (EquipmentInventoryComponent)
		EquipmentInventoryComponent->UnequipItem(ItemSlot);
}

ASoulEquipment* USoulInventoryManagerComponent::GetEquipmentItem(ESoulEquipmentSlotType EquipmentSlotType)
{
	if (EquipmentInventoryComponent)
		return EquipmentInventoryComponent->GetEquipmentItem(EquipmentSlotType);
	
	return nullptr;
}

void USoulInventoryManagerComponent::AddToItems_Server_Implementation(const TArray<FSoulItemData>& Items, AActor* ActorToDestroy)
{
	EquipmentInventoryComponent->AddItems(Items);

	if (ActorToDestroy)
		ActorToDestroy->Destroy();
}