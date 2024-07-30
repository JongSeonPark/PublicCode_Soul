// Copyright 2024 ChickenGames All rights reserved.


#include "SoulAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Soul.h"
#include "SoulGameplayTags.h"

#include "Net/UnrealNetwork.h"

USoulAttributeSet::USoulAttributeSet()
{
	// TODO: 추후 캐릭터별로 지정할 수 있는 방법으로 지정한다.
	InitMaxPoiseHealth(3.f);
	InitPoiseHealth(3.f);
	InitMaxStanceHealth(10.f);
	InitStanceHealth(10.f);
}

void USoulAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Base Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Faith, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Agility, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Luck, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, MaxStanceHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, MaxPoiseHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, PhysicalPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, SpellPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, DivinePower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, PhysicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, MagicDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, ItemDiscovery, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Poise, COND_None, REPNOTIFY_Always);

	// fluctuating Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, StanceHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, PoiseHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Stamina, COND_None, REPNOTIFY_Always);

}

bool USoulAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	// SOUL_LOG(Display, TEXT("[%s %f] %hs"), *Data.EvaluatedData.Attribute.AttributeName, Data.EvaluatedData.Magnitude, __FUNCTION__);
	if (!Super::PreGameplayEffectExecute(Data))
		return false;

	return true;
}

void USoulAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// SOUL_LOG(Display, TEXT("[%s %f] %hs"), *Data.EvaluatedData.Attribute.AttributeName, Data.EvaluatedData.Magnitude, __FUNCTION__);
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(GetHealth() - Data.EvaluatedData.Magnitude);
		SetDamage(0.f);
		if (GetHealth() <= 0)
			GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(SoulGameplayTags::Ability_Dead));	
	}
	else if (Data.EvaluatedData.Attribute == GetStanceDamageAttribute())
	{
		if (GetStanceHealth() > Data.EvaluatedData.Magnitude)
		{
			SetStanceHealth(GetStanceHealth() - Data.EvaluatedData.Magnitude);
		}
		else
		{
			SetStanceHealth(GetMaxStanceHealth());
			GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(SoulGameplayTags::Status_Collapsing));	
		}
		SetStanceDamage(0.f);
	}
	else if (Data.EvaluatedData.Attribute == GetPoiseDamageAttribute())
	{
		if (GetPoiseHealth() > Data.EvaluatedData.Magnitude)
		{
			SetPoiseHealth(GetPoiseHealth() - Data.EvaluatedData.Magnitude);
		}
		else
		{
			SetPoiseHealth(GetMaxPoiseHealth());
			FGameplayTagContainer StaggerTag(SoulGameplayTags::Status_Stagger);
			GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(StaggerTag);	
		}
		SetPoiseDamage(0.f);
	}
}

void USoulAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	// SOUL_LOG(Display, TEXT("[%s %f] %hs"), *Attribute.GetName(), NewValue, __FUNCTION__);
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void USoulAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// SOUL_LOG(Display, TEXT("[%s %f] %hs"), *Attribute.GetName(), NewValue, __FUNCTION__);
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void USoulAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	// SOUL_LOG(Display, TEXT("[%s %f %f] %hs"), *Attribute.GetName(), OldValue, NewValue, __FUNCTION__);
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void USoulAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	else if (Attribute == GetPoiseHealthAttribute())
	{
		NewValue = FMath::Min(NewValue, GetMaxPoiseHealth());
	}
	else if (Attribute == GetStanceHealthAttribute())
	{
		NewValue = FMath::Min(NewValue, GetMaxStanceHealth());
	}
}

#pragma region OnRep

void USoulAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Strength, OldStrength);
}

void USoulAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Intelligence, OldIntelligence);
}

void USoulAttributeSet::OnRep_Faith(const FGameplayAttributeData& OldFaith)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Faith, OldFaith);
}

void USoulAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Vitality, Vitality);
}

void USoulAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Agility, OldAgility);
}

void USoulAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Luck, OldLuck);
}

void USoulAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, MaxHealth, OldMaxHealth);
}

void USoulAttributeSet::OnRep_MaxStanceHealth(const FGameplayAttributeData& OldMaxStanceHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, MaxStanceHealth, OldMaxStanceHealth);
}

void USoulAttributeSet::OnRep_MaxPoiseHealth(const FGameplayAttributeData& OldMaxPoiseHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, MaxPoiseHealth, OldMaxPoiseHealth);
}

void USoulAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, MaxMana, OldMaxMana);
}

void USoulAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, MaxStamina, OldMaxStamina);
}

void USoulAttributeSet::OnRep_PhysicalPower(const FGameplayAttributeData& OldPhysicalPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, PhysicalPower, OldPhysicalPower);
}

void USoulAttributeSet::OnRep_SpellPower(const FGameplayAttributeData& OldSpellPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, SpellPower, OldSpellPower);
}

void USoulAttributeSet::OnRep_DivinePower(const FGameplayAttributeData& OldDivinePower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, DivinePower, OldDivinePower);
}

void USoulAttributeSet::OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, PhysicalDefense, OldPhysicalDefense);
}

void USoulAttributeSet::OnRep_MagicDefense(const FGameplayAttributeData& OldMagicDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, MagicDefense, OldMagicDefense);
}

void USoulAttributeSet::OnRep_ItemDiscovery(const FGameplayAttributeData& OldItemDiscovery)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, ItemDiscovery, OldItemDiscovery);
}

void USoulAttributeSet::OnRep_Poise(const FGameplayAttributeData& OldPoise)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Poise, OldPoise);
}

void USoulAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Health, OldHealth);
}

void USoulAttributeSet::OnRep_StanceHealth(const FGameplayAttributeData& OldStanceHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, StanceHealth, OldStanceHealth);
}

void USoulAttributeSet::OnRep_PoiseHealth(const FGameplayAttributeData& OldPoiseHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, PoiseHealth, OldPoiseHealth);
}

void USoulAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Mana, OldMana);
}

void USoulAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Stamina, OldStamina);
}

#pragma endregion
