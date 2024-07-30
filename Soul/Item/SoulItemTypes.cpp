// Copyright 2024 ChickenGames All rights reserved.


#include "SoulItemTypes.h"

#include "AbilitySystem/Abilities/SoulGameplayAbility.h"
#include "Equipments/SoulEquipment.h"

bool FEquipmentItemData::IsEmpty() const
{
	return SoulEquipmentSlotType == ESoulEquipmentSlotType::None || !EquipmentClass;
}

bool FConsumableItemData::IsEmpty() const
{
	return !AbilityEventTag.IsValid();
}
