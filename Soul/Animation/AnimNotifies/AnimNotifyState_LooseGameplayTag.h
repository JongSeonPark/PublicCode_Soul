// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_LooseGameplayTag.generated.h"

/**
 * 해당 영역동안 LooseGameplayTag를 적용시킴
 */
UCLASS()
class SOUL_API UAnimNotifyState_LooseGameplayTag : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;
	UPROPERTY(EditAnywhere, Category = "GameplayTag")
	FGameplayTagContainer GameplayTags;
	
};
