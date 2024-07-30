// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FootStep.generated.h"

/**
 * 발소리, 이펙트 적용 Notify
 */
UCLASS()
class SOUL_API UAnimNotify_FootStep : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// 인간형은 foot_l_Socket, foot_r_Socket로 나눌 것으로 예상되지만, 4족이나 다른 스탭에 관한 경우 추가될 수 있음.
	UPROPERTY(EditAnywhere, Category = "FootStep", meta = (AnimNotifyBoneName = "true"))
	FName FootBoneName = "foot_l_Socket";
	UPROPERTY(EditAnywhere, Category = "FootStep")
	float TraceLength = 10.f;
	UPROPERTY(EditAnywhere, Category = "FootStep")
	bool bDrawDebug = false;
	
};
