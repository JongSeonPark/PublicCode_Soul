// Copyright 2024 ChickenGames All rights reserved.


#include "SoulWorldItem.h"

#include "Soul.h"
#include "Character/SoulCharacterBase.h"
#include "Components/BoxComponent.h"
#include "Game/SoulGameInstance.h"
#include "Inventory/SoulInventoryManagerComponent.h"


ASoulWorldItem::ASoulWorldItem()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);
}

void ASoulWorldItem::Interaction_Implementation(ASoulCharacterBase* InteractionCharacter)
{
	InteractionCharacter->GetInventoryManagerComponent()->AddToItems_Server(Items, this);
}

FText ASoulWorldItem::GetInteractionInfo_Implementation()
{
	return InteractionInfo;
}
