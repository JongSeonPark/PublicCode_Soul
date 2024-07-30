// Copyright 2024 ChickenGames All rights reserved.


#include "AnimNotifyState_ActiveCriticalHitArea.h"

#include "Combat/CriticalHitAreaComponent.h"

void UAnimNotifyState_ActiveCriticalHitArea::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* Actor = MeshComp->GetOwner();
	if (!Actor) return;
	TArray<UCriticalHitAreaComponent*> CriticalHitAreaComponents;
	Actor->GetComponents(UCriticalHitAreaComponent::StaticClass(), CriticalHitAreaComponents);
	for (UCriticalHitAreaComponent* CriticalHitAreaComponent : CriticalHitAreaComponents)
	{
		if (CriticalHitAreaComponent->GetCriticalHitAreaType() == CriticalHitAreaType && !CriticalHitAreaComponent->IsActiveWhenBeginPlay())
		{
			CriticalHitAreaComponent->SetActive(true);
		}
	}
}

void UAnimNotifyState_ActiveCriticalHitArea::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* Actor = MeshComp->GetOwner();
	if (!Actor) return;
	TArray<UCriticalHitAreaComponent*> CriticalHitAreaComponents;
	Actor->GetComponents(UCriticalHitAreaComponent::StaticClass(), CriticalHitAreaComponents);
	for (UCriticalHitAreaComponent* CriticalHitAreaComponent : CriticalHitAreaComponents)
	{
		if (CriticalHitAreaComponent->GetCriticalHitAreaType() == CriticalHitAreaType && !CriticalHitAreaComponent->IsActiveWhenBeginPlay())
		{
			CriticalHitAreaComponent->SetActive(false);
		}
	}
}
