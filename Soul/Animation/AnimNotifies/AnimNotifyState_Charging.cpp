// Copyright 2024 ChickenGames All rights reserved.


#include "AnimNotifyState_Charging.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "SoulGameplayTags.h"
#include "AbilitySystem/SoulAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "Character/SoulPlayerCharacter.h"

void UAnimNotifyState_Charging::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* Actor = MeshComp->GetOwner();
	if (!Actor) return;
	if (!Actor->HasLocalNetOwner()) return;
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!AbilitySystemComponent) return;

	TotalTime = TotalDuration;
	ElapsedTime = 0.f;
	// SOUL_LOG(Log, TEXT("ChargeStart: Total: %f"), TotalTime);
	AbilitySystemComponent->AddLooseGameplayTag(SoulGameplayTags::Status_Attack_Charging);
}

void UAnimNotifyState_Charging::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AActor* Actor = MeshComp->GetOwner();
	if (!Actor) return;
	if (!Actor->HasLocalNetOwner()) return;
	USoulAbilitySystemComponent* AbilitySystemComponent = USoulAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!AbilitySystemComponent) return;

	const FAnimMontageInstance* AnimMontageInstance = MeshComp->GetAnimInstance()->GetActiveMontageInstance();
	const float PlayRate = AnimMontageInstance ? AnimMontageInstance->GetPlayRate() : 1.f;
	ElapsedTime += FrameDeltaTime * PlayRate;
	const float ChargeRate = ElapsedTime / TotalTime;
	
	// SOUL_LOG(Log, TEXT("PlayRate: %f, Charing.. ElapsedTime = %f, Total: %f, Current: %f"), PlayRate, ElapsedTime, TotalTime, EventReference.GetCurrentAnimationTime());
	
	AbilitySystemComponent->ChargeRate = ChargeRate;

}

void UAnimNotifyState_Charging::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* Actor = MeshComp->GetOwner();
	if (!Actor) return;
	if (!Actor->HasLocalNetOwner()) return;
	USoulAbilitySystemComponent* AbilitySystemComponent = USoulAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!AbilitySystemComponent) return;
	AbilitySystemComponent->RemoveLooseGameplayTag(SoulGameplayTags::Status_Attack_Charging);

	ElapsedTime = 0.f;
	AbilitySystemComponent->ChargeRate = 0.f;
	
	// float ChargeRate = ElapsedTime / TotalTime;
	// SOUL_LOG(Log, TEXT("ChargeRate = %f"), ChargeRate);
}