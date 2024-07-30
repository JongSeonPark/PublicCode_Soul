// Copyright 2024 ChickenGames All rights reserved.


#include "GameplayAbility_CriticalHit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Soul.h"
#include "SoulGameplayTags.h"
#include "Character/SoulCharacterBase.h"
#include "Combat/CriticalHitAreaComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UGameplayAbility_CriticalHit::UGameplayAbility_CriticalHit()
{
	AbilityTags.AddTag(SoulGameplayTags::Status_Attack_CriticalHit);
	ActivationOwnedTags.AddTag(SoulGameplayTags::Status_Attack_CriticalHit);
	ActivationOwnedTags.AddTag(SoulGameplayTags::Block_SoulAction);
	// 선입력 순간에 공격이 되는 경우가 있음.
	ActivationBlockedTags.AddTag(SoulGameplayTags::Status_Attack);
}

bool UGameplayAbility_CriticalHit::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                      const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	return GetOverlapComponents(ActorInfo->AvatarActor.Get()).Num() > 0;
}

void UGameplayAbility_CriticalHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
	ASoulCharacterBase* AvatarActor = Cast<ASoulCharacterBase>(OwnerInfo->AvatarActor.Get());
	if (!AvatarActor) return;
	TArray<UPrimitiveComponent*> OverlapComponents = GetOverlapComponents(AvatarActor);
	for (UPrimitiveComponent* OverlapComponent : OverlapComponents)
	{
		ASoulCharacterBase* OverlapActor = Cast<ASoulCharacterBase>(OverlapComponent->GetOwner());
		if (!OverlapActor) continue;
		if (UAbilitySystemComponent* ASC = OverlapActor->GetAbilitySystemComponent())
		{
			ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(SoulGameplayTags::Status_BeCriticalHit));
			
			AvatarActor->SetActorRotation((OverlapActor->GetActorLocation() - AvatarActor->GetActorLocation()).Rotation());
			OverlapActor->SetActorRotation((AvatarActor->GetActorLocation() - OverlapActor->GetActorLocation()).Rotation());
			AvatarActor->CriticalHitTarget = OverlapActor;
			return;
		}
	}
}

void UGameplayAbility_CriticalHit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (ASoulCharacterBase* AvatarActor = Cast<ASoulCharacterBase>(ActorInfo->AvatarActor.Get()))
	{
		AvatarActor->CriticalHitTarget = nullptr;
	}
}

TArray<UPrimitiveComponent*> UGameplayAbility_CriticalHit::GetOverlapComponents(AActor* Avatar) const
{
	TArray<UPrimitiveComponent*> Results;
	if (const ASoulCharacterBase* Character = Cast<ASoulCharacterBase>(Avatar))
	{
		TArray<UPrimitiveComponent*> OverlapComponents;
		UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
		UKismetSystemLibrary::ComponentOverlapComponents(
			Capsule,
			Capsule->GetComponentTransform(),
			TArray<TEnumAsByte<EObjectTypeQuery>>(),
			UCriticalHitAreaComponent::StaticClass(),
			TArray<AActor*>(),
			OverlapComponents);

		for (UPrimitiveComponent* OverlapComponent : OverlapComponents)
		{
			// 무적 상태 제외.
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlapComponent->GetOwner());
			if (!TargetASC || TargetASC->HasAnyMatchingGameplayTags(SoulGameplayTags::InvincibleTags)) continue;
			
			// 바라보는 방향에 있는 액터만 적용
			const FVector Dir = (OverlapComponent->GetOwner()->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal();
			const float Dot = Character->GetActorForwardVector().Dot(Dir);
			// 각도 조절이 필요하면 조건 범위 수정.(0 이상일 시 180도)
			if (Dot > 0.f)
				Results.Add(OverlapComponent);
		}
	}

	return Results;
}
