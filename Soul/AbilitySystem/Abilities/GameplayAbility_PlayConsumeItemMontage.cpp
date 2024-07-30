// Copyright 2024 ChickenGames All rights reserved.


#include "GameplayAbility_PlayConsumeItemMontage.h"

#include "AbilitySystemComponent.h"
#include "SoulGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/SoulCharacterBase.h"
#include "Inventory/SoulEquipmentInventoryComponent.h"

UGameplayAbility_PlayConsumeItemMontage::UGameplayAbility_PlayConsumeItemMontage()
{
}

bool UGameplayAbility_PlayConsumeItemMontage::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                                 const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	// 아이템이 1개 이상일 때만 실행 가능
	if (ASoulCharacterBase* AvatarCharacter = Cast<ASoulCharacterBase>(ActorInfo->AvatarActor))
	{
		TMap<FName, FSoulItemData> Items = AvatarCharacter->GetEquipmentInventoryComponent()->GetInventoryItemMap();
		return Items.Contains(ItemID) && Items[ItemID].Amount > 0;
	}
	return false;
}

void UGameplayAbility_PlayConsumeItemMontage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Amount = FMath::Floor(TriggerEventData->EventMagnitude);
	
	WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, SoulGameplayTags::Event_ConsumeItem);
	WaitGameplayEvent->EventReceived.AddDynamic(this, &UGameplayAbility_PlayConsumeItemMontage::OnConsumeEvent);
	WaitGameplayEvent->ReadyForActivation();
}

void UGameplayAbility_PlayConsumeItemMontage::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                            bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	WaitGameplayEvent->EndTask();
}

void UGameplayAbility_PlayConsumeItemMontage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                         bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	WaitGameplayEvent->EndTask();
}

void UGameplayAbility_PlayConsumeItemMontage::OnConsumeEvent(FGameplayEventData Payload)
{
	if (ASoulCharacterBase* AvatarCharacter = Cast<ASoulCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		TMap<FName, FSoulItemData> Items = AvatarCharacter->GetEquipmentInventoryComponent()->GetInventoryItemMap();
		if (Items.Contains(ItemID) && Items[ItemID].Amount >= Amount)
		{
			if (K2_HasAuthority())
			{
				// 섭취 횟수(Amount)를 Level로 적용하겠음.
				for (const TSubclassOf<UGameplayEffect> GameplayEffect : AdjustEffects)
				{
					UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
					FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
					FGameplayEffectContext* A = EffectContextHandle.Get();
					FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, Amount, EffectContextHandle);
					K2_ApplyGameplayEffectSpecToOwner(EffectSpecHandle);
				}

				AvatarCharacter->GetEquipmentInventoryComponent()->RemoveItem(FSoulItemData(ItemID, Amount));
			}
		}
		else
		{
			// 실패 에니메이션 적용
			PlayMontageAndWaitTask->EndTask();
			PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, PlayMontageData_Fail.MontageToPlay, PlayMontageData_Fail.PlayRate, PlayMontageData_Fail.SectionName);
			PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UGameplayAbility_PlayConsumeItemMontage::K2_CancelAbility);
			PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_PlayConsumeItemMontage::K2_CancelAbility);
			PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_PlayConsumeItemMontage::K2_EndAbility);
			PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UGameplayAbility_PlayConsumeItemMontage::K2_EndAbility);
			PlayMontageAndWaitTask->ReadyForActivation();
		}
	}
}
