// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SetMovementMode.generated.h"

/**
 * 해당 영역동안 Movement모드를 설정.
 * 점프를 위해서 제작되었음.
 */
UCLASS()
class SOUL_API UAnimNotifyState_SetMovementMode : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = "MovementMode")
	TEnumAsByte<enum EMovementMode> NewMovementMode;
	UPROPERTY(EditAnywhere, Category = "MovementMode")
	uint8 NewCustomMode = 0;
	
};
