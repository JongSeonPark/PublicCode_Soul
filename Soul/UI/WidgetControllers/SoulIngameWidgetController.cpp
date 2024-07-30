// Copyright 2024 ChickenGames All rights reserved.


#include "SoulIngameWidgetController.h"

#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/SoulAttributeSet.h"

void USoulIngameWidgetController::BindAllEvent()
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

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnManaChange.Broadcast(ChangeData.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnMaxManaChange.Broadcast(ChangeData.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnStaminaChange.Broadcast(ChangeData.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnMaxStaminaChange.Broadcast(ChangeData.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetStanceHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnStanceHealthChange.Broadcast(ChangeData.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMaxStanceHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnMaxStanceHealthChange.Broadcast(ChangeData.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetPoiseHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnPoiseHealthChange.Broadcast(ChangeData.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMaxPoiseHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& ChangeData)
		{
			OnMaxPoiseHealthChange.Broadcast(ChangeData.NewValue);
		});

}

void USoulIngameWidgetController::BroadcastInitValue()
{
	OnHealthChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetHealthAttribute()));
	OnMaxHealthChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetMaxHealthAttribute()));
	
	OnManaChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetManaAttribute()));
	OnMaxManaChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetMaxManaAttribute()));
	
	OnStaminaChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetStaminaAttribute()));
	OnMaxStaminaChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetMaxStaminaAttribute()));
	
	OnStanceHealthChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetStanceHealthAttribute()));
	OnMaxStanceHealthChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetMaxStanceHealthAttribute()));
	
	OnPoiseHealthChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetPoiseHealthAttribute()));
	OnMaxPoiseHealthChange.Broadcast(AbilitySystemComponent->GetNumericAttribute(USoulAttributeSet::GetMaxPoiseHealthAttribute()));
}
