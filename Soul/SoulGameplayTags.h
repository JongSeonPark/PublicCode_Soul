// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace SoulGameplayTags
{
	SOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	SOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	SOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	SOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	SOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	SOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	
	SOUL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_CriticalHit);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage_Stance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage_Poise);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Movement_Walking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Movement_Sprinting);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Attack_Charging);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Attack_Buffering);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Attack_CriticalHit);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Guard);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Dying);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Stagger);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_HyperArmor);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Collapsing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_GuardBreak);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_BeCriticalHit);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Action_LockOn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Action_Rolling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Action_Jumping);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Falling);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Force_LockOnUpdate);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cancel_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cancel_Attack_WallImpact);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cancel_Attack_Stagger);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cancel_Attack_Collapsing);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Block_SoulAction);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Consume_Potion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Consume_LargePotion);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Damage_Guard);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_LockOnTargeted);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_ConsumeItem);
	

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Right);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Right_Normal);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Right_Strong);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Left);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Left_Normal);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Left_Strong);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo_Right_Normal);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo_Right_Strong);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo_Left_Normal);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo_Left_Strong);

	extern const FGameplayTagContainer InvincibleTags;

	// InputTag로 검색
	extern const TMap<FGameplayTag, FGameplayTag> ComboToInputMap;
}