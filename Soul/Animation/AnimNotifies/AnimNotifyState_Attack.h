// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Item/SoulItemTypes.h"
#include "AnimNotifyState_Attack.generated.h"

class UNiagaraSystem;
class ASoulWeapon;
class UFXSystemComponent;

/**
 * 공격에서 검의 Trail 및 충돌처리를 발생시키는 영역 지정.
 */
UCLASS()
class SOUL_API UAnimNotifyState_Attack : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = Attack)
	float DamageRate = 1.f;
	UPROPERTY(EditAnywhere, Category = Attack)
	float StanceDamageRate = 1.f;
	UPROPERTY(EditAnywhere, Category = Attack)
	float PoiseDamageRate = 1.f;
	UPROPERTY(EditAnywhere, Category = Attack)
	FName AttachSocketName = FName("Root");
	UPROPERTY(EditAnywhere, Category = Attack)
	ESoulEquipmentSlotType WeaponSlotType = ESoulEquipmentSlotType::MainHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NiagaraSystem)
	TObjectPtr<UNiagaraSystem> Trail;
	
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	UFXSystemComponent* GetSpawnedEffect(ASoulWeapon* Weapon);
	
protected:
	virtual UFXSystemComponent* SpawnEffect(ASoulWeapon* Weapon, UAnimSequenceBase* Animation) const;
	bool ValidateParameters(ASoulWeapon* Weapon) const;
	ASoulWeapon* GetWeapon(USkeletalMeshComponent* MeshComp) const;
	FORCEINLINE FName GetSpawnedComponentTag()const { return GetFName(); }
 
	
};
