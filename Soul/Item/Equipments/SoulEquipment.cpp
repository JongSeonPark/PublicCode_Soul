// Copyright 2024 ChickenGames All rights reserved.


#include "SoulEquipment.h"
#include "AbilitySystemComponent.h"
#include "EquipmentDatas.h"
#include "AbilitySystem/SoulAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Abilities/SoulGameplayAbility.h"

void ASoulEquipment::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent = USoulAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
}

void ASoulEquipment::Equip(USoulAbilitySystemComponent* InAbilitySystemComponent)
{
	AbilitySystemComponent = InAbilitySystemComponent;
	
	// 가지고 있는 어빌리티를 캐릭터에 부여
	if (AbilitySystemComponent && AbilitySystemComponent->IsOwnerActorAuthoritative() && EquipmentAbilityDataAsset)
	{
		TArray<TSubclassOf<USoulGameplayAbility>> AbilityClasses;

		for (FSoulWeaponAbilityWithInputAction WeaponAbilityData : EquipmentAbilityDataAsset->WeaponAbilityDatas)
		{
			if (WeaponAbilityData.InputAction)
			{
				// Warning! 작동 우선순위 순서대로 작업되어야 함.
				if (WeaponAbilityData.JumpAbilityClass)
				{
					const FGameplayAbilitySpec Spec = AbilitySystemComponent->BuildSoulAbilitySpecFromClass(WeaponAbilityData.JumpAbilityClass, 0, WeaponAbilityData.InputAction);
					AbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(Spec));
				}
				if (WeaponAbilityData.DashAbilityClass)
				{
					const FGameplayAbilitySpec Spec = AbilitySystemComponent->BuildSoulAbilitySpecFromClass(WeaponAbilityData.DashAbilityClass, 0, WeaponAbilityData.InputAction);
					AbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(Spec));
				}
				if (WeaponAbilityData.CriticalAbilityClass)
				{
					const FGameplayAbilitySpec Spec = AbilitySystemComponent->BuildSoulAbilitySpecFromClass(WeaponAbilityData.CriticalAbilityClass, 0, WeaponAbilityData.InputAction);
					AbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(Spec));
				}
				if (WeaponAbilityData.DefaultAbilityClass)
				{
					const FGameplayAbilitySpec Spec = AbilitySystemComponent->BuildSoulAbilitySpecFromClass(WeaponAbilityData.DefaultAbilityClass, 0, WeaponAbilityData.InputAction);
					AbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(Spec));
				}
			}
		}
		for (const TSubclassOf<UGameplayAbility> AbilityClass : EquipmentAbilityDataAsset->MiscAbilityClasses)
		{
			const FGameplayAbilitySpec Spec = AbilityClass;
			AbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(Spec));
		}
	}

}

void ASoulEquipment::Unequip()
{
	if (AbilitySystemComponent && AbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		for (FGameplayAbilitySpecHandle AbilitySpecHandle : AbilitySpecHandles)
		{
			AbilitySystemComponent->ClearAbility(AbilitySpecHandle);
		}
		AbilitySpecHandles.Empty();
	}
}
