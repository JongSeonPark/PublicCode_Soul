// Copyright 2024 ChickenGames All rights reserved.


#include "SoulWorldHealthBarController.h"

#include "SoulGameplayTags.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/SoulAttributeSet.h"

void USoulWorldHealthBarController::BindAllEvent()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnHealthChange.Broadcast(ChangeData.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnMaxHealthChange.Broadcast(ChangeData.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetDamageAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnDamage.Broadcast(ChangeData.NewValue);
		});
}

void USoulWorldHealthBarController::BroadcastInitValue()
{
	const float Health = AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetHealthAttribute());
	const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetMaxHealthAttribute());
	OnHealthChange.Broadcast(Health);
	OnMaxHealthChange.Broadcast(MaxHealth);

	OnTargeted.Broadcast(AbilitySystemComponent->HasMatchingGameplayTag(SoulGameplayTags::Event_LockOnTargeted));
}
