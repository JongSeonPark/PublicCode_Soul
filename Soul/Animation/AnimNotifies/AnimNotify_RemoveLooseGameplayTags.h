// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_RemoveLooseGameplayTags.generated.h"

UCLASS()
class SOUL_API UAnimNotify_RemoveLooseGameplayTags : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;
	UPROPERTY(EditAnywhere, Category = "GameplayTag")
	FGameplayTagContainer GameplayTags;
};
