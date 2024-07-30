// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameplayAbilitySet.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "SoulAbilitySystemComponent.h"
#include "Abilities/SoulGameplayAbility.h"

void USoulGameplayAbilitySet::GiveAbilities(USoulAbilitySystemComponent* AbilitySystemComponent) const
{
	for (FSoulGameplayAbilityBindInfo AbilityBindInfo : Abilities)
	{
		// 소울 게임에 레벨을 사용하는 어빌리티는 없을 것 같음.
		FGameplayAbilitySpec AbilitySpec = AbilitySystemComponent->BuildSoulAbilitySpecFromClass(AbilityBindInfo.GameplayAbilityClass, 0, AbilityBindInfo.InputAction);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}

	for (TSubclassOf<UGameplayAbility> GrantAbility : NoBindAbilties)
	{
		FGameplayAbilitySpec AbilitySpec = AbilitySystemComponent->BuildAbilitySpecFromClass(GrantAbility);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void USoulGameplayAbilitySet::BindInputs(UInputComponent* InputComponent, USoulAbilitySystemComponent* AbilitySystemComponent) const
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent) return;

	TArray<UInputAction*> InputActions;
	BindInputToActivateTags.GenerateValueArray(InputActions);
	for (FSoulGameplayAbilityBindInfo AbilityBindInfo : Abilities)
	{
		InputActions.Add(AbilityBindInfo.InputAction);
	}

	for (UInputAction* InputAction : InputActions)
	{
		// 대부분 Input Start일것으로 가정, Trigger로 해야할 어빌리티 발생시 수정.
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Triggered,
			AbilitySystemComponent,
			&USoulAbilitySystemComponent::AbilityLocalInputTriggered,
			InputAction
		);
		
		EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Completed,
			AbilitySystemComponent,
			&USoulAbilitySystemComponent::AbilityLocalInputCompleted,
			InputAction
		);
	}
}
