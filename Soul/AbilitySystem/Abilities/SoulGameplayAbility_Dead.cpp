// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameplayAbility_Dead.h"

#include "AbilitySystemComponent.h"
#include "SoulGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

USoulGameplayAbility_Dead::USoulGameplayAbility_Dead()
{
	AbilityTags.AddTag(SoulGameplayTags::Ability_Dead);
	ActivationOwnedTags.AddTag(SoulGameplayTags::Status_Dying);
}

void USoulGameplayAbility_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, MontageData.MontageToPlay, MontageData.PlayRate, MontageData.SectionName);
	// PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &USoulGameplayAbility_Dead::K2_CancelAbility);
	// PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &USoulGameplayAbility_Dead::K2_CancelAbility);
	PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &USoulGameplayAbility_Dead::OnFinishAnim);
	PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &USoulGameplayAbility_Dead::OnFinishAnim);
	PlayMontageAndWaitTask->ReadyForActivation();
}

void USoulGameplayAbility_Dead::OnFinishAnim()
{
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(SoulGameplayTags::Status_Dead);
	K2_EndAbility();
}
