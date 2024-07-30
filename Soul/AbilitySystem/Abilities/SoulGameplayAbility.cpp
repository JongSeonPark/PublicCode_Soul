// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameplayAbility.h"

#include "AbilitySystemComponent.h"

void USoulGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	// 추가시 관련 Ability도 자동으로 추가시킴.
	if (ActorInfo->IsNetAuthority())
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		for (const TSubclassOf<UGameplayAbility> ChildAbility : GetChildAbilities())
		{
			FGameplayAbilitySpec AbilitySpec = ASC->BuildAbilitySpecFromClass(ChildAbility, GetAbilityLevel(), 0);
			if (USoulGameplayAbility* SoulAbility = Cast<USoulGameplayAbility>(AbilitySpec.Ability))
			{
				SoulAbility->SetParentInputAction(InputAction);
			}
			ChildAbilitySpecHandles.Add(ASC->GiveAbility(AbilitySpec));
		}
	}
}

void USoulGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	if (ActorInfo->IsNetAuthority())
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		for (FGameplayAbilitySpecHandle ChildAbilitySpecHandle : ChildAbilitySpecHandles)
		{
			ASC->ClearAbility(ChildAbilitySpecHandle);
		
		}
		ChildAbilitySpecHandles.Empty();
	}
}

void USoulGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (bCancelToInputCompleted && ActorInfo && ActorInfo->AvatarActor != nullptr)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}