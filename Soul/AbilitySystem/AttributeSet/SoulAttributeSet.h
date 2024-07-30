// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "SoulAttributeSet.generated.h"

/**
 * Soullike Game에 사용될 Attribute 정의.
 */
UCLASS()
class SOUL_API USoulAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	USoulAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

private:
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

public:
#pragma region Base Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Base Attribute", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Strength)
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UPROPERTY(BlueprintReadOnly, Category = "Base Attribute", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Intelligence)
	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);

	UPROPERTY(BlueprintReadOnly, Category = "Base Attribute", ReplicatedUsing = OnRep_Faith)
	FGameplayAttributeData Faith;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Faith)
	UFUNCTION()
	virtual void OnRep_Faith(const FGameplayAttributeData& OldFaith);

	UPROPERTY(BlueprintReadOnly, Category = "Base Attribute", ReplicatedUsing = OnRep_Vitality)
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Vitality)
	UFUNCTION()
	virtual void OnRep_Vitality(const FGameplayAttributeData& OldVitality);

	UPROPERTY(BlueprintReadOnly, Category = "Base Attribute", ReplicatedUsing = OnRep_Agility)
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Agility)
	UFUNCTION()
	virtual void OnRep_Agility(const FGameplayAttributeData& OldAgility);

	UPROPERTY(BlueprintReadOnly, Category = "Base Attribute", ReplicatedUsing = OnRep_Luck)
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Luck)
	UFUNCTION()
	virtual void OnRep_Luck(const FGameplayAttributeData& OldLuck);
#pragma endregion

#pragma region Secondary Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxHealth)
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_MaxStanceHealth)
	FGameplayAttributeData MaxStanceHealth;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxStanceHealth)
	UFUNCTION()
	virtual void OnRep_MaxStanceHealth(const FGameplayAttributeData& OldMaxStanceHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_MaxPoiseHealth)
	FGameplayAttributeData MaxPoiseHealth;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxPoiseHealth)
	UFUNCTION()
	virtual void OnRep_MaxPoiseHealth(const FGameplayAttributeData& OldMaxPoiseHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxMana)
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxStamina)
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	// 물리 데미지
	// 마법 데미지
	// 신앙 데미지
	// 방어력
	// 마법 저항
	// 아이템 획득률
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_PhysicalPower)
	FGameplayAttributeData PhysicalPower;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PhysicalPower)
	UFUNCTION()
	virtual void OnRep_PhysicalPower(const FGameplayAttributeData& OldPhysicalPower);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_SpellPower)
	FGameplayAttributeData SpellPower;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, SpellPower)
	UFUNCTION()
	virtual void OnRep_SpellPower(const FGameplayAttributeData& OldSpellPower);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_DivinePower)
	FGameplayAttributeData DivinePower;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, DivinePower)
	UFUNCTION()
	virtual void OnRep_DivinePower(const FGameplayAttributeData& OldDivinePower);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_PhysicalDefense)
	FGameplayAttributeData PhysicalDefense;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PhysicalDefense)
	UFUNCTION()
	virtual void OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_MagicDefense)
	FGameplayAttributeData MagicDefense;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MagicDefense)
	UFUNCTION()
	virtual void OnRep_MagicDefense(const FGameplayAttributeData& OldMagicDefense);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_ItemDiscovery)
	FGameplayAttributeData ItemDiscovery;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, ItemDiscovery)
	UFUNCTION()
	virtual void OnRep_ItemDiscovery(const FGameplayAttributeData& OldItemDiscovery);
	
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attribute", ReplicatedUsing = OnRep_Poise)
	FGameplayAttributeData Poise;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Poise)
	UFUNCTION()
	virtual void OnRep_Poise(const FGameplayAttributeData& OldPoise);
	
#pragma endregion

#pragma region Fluctuating Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Fluctuating Value", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Health)
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Fluctuating Value", ReplicatedUsing = OnRep_StanceHealth)
	FGameplayAttributeData StanceHealth;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, StanceHealth)
	UFUNCTION()
	virtual void OnRep_StanceHealth(const FGameplayAttributeData& OldStanceHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Fluctuating Attribute", ReplicatedUsing = OnRep_PoiseHealth)
	FGameplayAttributeData PoiseHealth;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PoiseHealth)
	UFUNCTION()
	virtual void OnRep_PoiseHealth(const FGameplayAttributeData& OldPoiseHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Fluctuating Attribute", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Mana)
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UPROPERTY(BlueprintReadOnly, Category = "Fluctuating Attribute", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Stamina)
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
#pragma endregion

#pragma region Meta Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Damage)
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
	FGameplayAttributeData StanceDamage;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, StanceDamage)
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
	FGameplayAttributeData PoiseDamage;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PoiseDamage)
#pragma endregion

};
