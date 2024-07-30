// Copyright 2024 ChickenGames All rights reserved.


#include "SoulAnimInstance.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/SoulCharacterBase.h"

void USoulAnimInstance::NativeInitializeAnimation()
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TryGetPawnOwner()))
	{
		InitAbilitySystem(ASC);
	}
}

ASoulCharacterBase* USoulAnimInstance::TryGetOwningSoulCharacter() const
{
	USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
	if (AActor* OwnerActor = OwnerComponent->GetOwner())
	{
		return Cast<ASoulCharacterBase>(OwnerActor);
	}
	return nullptr;
}

void USoulAnimInstance::InitAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent)
{
	GameplayTagPropertyMap.Initialize(this, AbilitySystemComponent);
}
