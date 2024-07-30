// Copyright 2024 ChickenGames All rights reserved.


#include "SoulShield.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "SoulGameplayTags.h"
#include "Character/SoulCharacterBase.h"
#include "Components/BoxComponent.h"


ASoulShield::ASoulShield()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	SetRootComponent(Mesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	BoxComponent->SetupAttachment(Mesh);
}

void ASoulShield::BeginPlay()
{
	Super::BeginPlay();
	if (ASoulCharacterBase* OwnerCharacter = Cast<ASoulCharacterBase>(GetOwner()))
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("hand_Shield_l"));
	}
}

void ASoulShield::Equip(USoulAbilitySystemComponent* InAbilitySystemComponent)
{
	Super::Equip(InAbilitySystemComponent);
	GuardTagChangedDelegateHandle = AbilitySystemComponent->RegisterGameplayTagEvent(SoulGameplayTags::Status_Guard).AddUObject(this, &ASoulShield::OnGuardTagChanged);
}

void ASoulShield::Unequip()
{
	AbilitySystemComponent->UnregisterGameplayTagEvent(GuardTagChangedDelegateHandle, SoulGameplayTags::Status_Guard);
	Super::Unequip();
}

void ASoulShield::OnGuardTagChanged(const FGameplayTag Tag, int32 Count)
{
	BoxComponent->SetCollisionEnabled(Count > 0 ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	BoxComponent->SetGenerateOverlapEvents(Count > 0);
}
