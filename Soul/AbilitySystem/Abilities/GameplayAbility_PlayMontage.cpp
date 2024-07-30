// Copyright 2024 ChickenGames All rights reserved.


#include "AbilitySystem/Abilities/GameplayAbility_PlayMontage.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGameplayAbility_PlayMontage::UGameplayAbility_PlayMontage()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_PlayMontage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
	if (!HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo) || !CommitAbility(Handle, OwnerInfo, ActivationInfo)) return;

	PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, PlayMontageData.MontageToPlay, PlayMontageData.PlayRate, PlayMontageData.SectionName);
	PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UGameplayAbility_PlayMontage::K2_CancelAbility);
	PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_PlayMontage::K2_CancelAbility);
	PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_PlayMontage::K2_EndAbility);
	PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UGameplayAbility_PlayMontage::K2_EndAbility);
	PlayMontageAndWaitTask->ReadyForActivation();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked())
	{
		for (TSubclassOf<UGameplayEffect> EffectClassesWhileAnimating : GameplayEffectClassesWhileAnimating)
		{
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClassesWhileAnimating, 0, ContextHandle);
			ActiveGameplayEffectHandles.Add(ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data));
		}
	}
}

void UGameplayAbility_PlayMontage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked())
	{
		for (const FActiveGameplayEffectHandle ActiveGameplayEffectHandle : ActiveGameplayEffectHandles)
		{
			ASC->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle);
		}
	}
}