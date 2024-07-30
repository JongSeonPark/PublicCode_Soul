// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "SoulEquipment.h"
#include "AbilitySystem/Effects/SoulDamageGameplayEffect.h"
#include "GameFramework/Actor.h"
#include "Physics/SoulPhysicsInterface.h"
#include "SoulWeapon.generated.h"

#define ECC_Weapon ECollisionChannel::ECC_GameTraceChannel2

class UPhysicsDataAsset;
class USoulAttackDataAsset;
class ASoulEnemyCharacter;
class UNiagaraSystem;

UCLASS()
class SOUL_API ASoulWeapon : public ASoulEquipment, public ISoulPhysicsInterface
{
	GENERATED_BODY()
	
public:	
	ASoulWeapon();

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	USkeletalMeshComponent* GetMesh() const { return Mesh; }
	
	void BeginAttack(float InDamageRate, float InStanceDamageRate, float InPoiseDamageRate);
	void EndAttack();

	virtual UPhysicsDataAsset* GetPhysicsDataAsset() const override { return PhysicsDataAsset; }

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> AttackEffectClass = USoulDamageGameplayEffect::StaticClass();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta=(AllowPrivateAccess = "true"))
	bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, Category = "Physics", meta = (AllowPrivateAccess = true))
	TObjectPtr<UPhysicsDataAsset> PhysicsDataAsset;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	TSet<UAbilitySystemComponent*> AttackedEnemyASCs;
	
	// 공격 시작시 벽에 닿으면 공격을 취소.
	void CheckBlockedByWall();

	FTimerHandle WeaponScrachTimerHandle;
	void OnScrachUpdate();

	float DamageRate;
	float StanceDamageRate;
	float PoiseDamageRate;

	FGameplayEffectSpecHandle MakeDamageEffectSpec(const TSubclassOf<UGameplayEffect>& EffectClass, const FHitResult& SweepResult = FHitResult()) const;
	bool BoxTrace(TArray<FHitResult>& HitResults);

	void TryActPhysicsEffect(const FHitResult& HitResult) const;
	void TryActPhysicsEffect(const TArray<FHitResult>& HitResults) const;

};
