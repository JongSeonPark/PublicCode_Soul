// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Combat/CriticalHitAreaComponent.h"
#include "AnimNotifyState_ActiveCriticalHitArea.generated.h"

/**
 * 해당 애니메이션의 치명적 공격이 가능한 구간을 정함.
 * 초기에 CiriticalHitAreaComponent가 false라면, AnimNotifyState에서 Active가 토글됨.
 */
UCLASS()
class SOUL_API UAnimNotifyState_ActiveCriticalHitArea : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = CiriticalHit)
	ECriticalHitAreaType CriticalHitAreaType;
};
