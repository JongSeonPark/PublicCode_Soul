// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Effects/SoulDamageGameplayEffect.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CriticalHitDamage.generated.h"

class UGameplayEffect;
/**
 * 소울라이크의 치명적 공격에서 대미지 적용 시점을 정하기 위한 노티파이
 */
UCLASS()
class SOUL_API UAnimNotify_CriticalHitDamage : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = Attack)
	float DamageRate = 1.f;
	// 웬만하면 사용되지 않음.
	UPROPERTY(EditAnywhere, Category = Attack)
	float StanceDamageRate = 0.f;
	// 웬만하면 사용되지 않음.
	UPROPERTY(EditAnywhere, Category = Attack)
	float PoiseDamageRate = 0.f;

	UPROPERTY(EditAnywhere, Category = Attack)
	TSubclassOf<UGameplayEffect> AttackEffectClass = USoulDamageGameplayEffect::StaticClass();
	
};
