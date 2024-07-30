// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

class UAbilitySystemComponent;
class ULockOnPointComponent;

/**
 * LockOn Update를 실행하기 위한 컴포넌트 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULockOnComponent();

protected:
	virtual void BeginPlay() override;
	virtual void LockOnUpdate(float DeltaTime);
	TObjectPtr<ULockOnPointComponent> LockOnPoint;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 락온 타겟을 지정한다. null일시 취소.
	UFUNCTION(BlueprintCallable)
	virtual void SetLockOnTarget(ULockOnPointComponent* InLockOnPoint);
	UFUNCTION(Server, Reliable)
	virtual void SetLockOnTarget_Server(ULockOnPointComponent* InLockOnPoint);
	UFUNCTION(BlueprintCallable)
	ULockOnPointComponent* GetLockOnTarget() const { return LockOnPoint; }

	TObjectPtr<ACharacter> OwnerCharacter;
	TObjectPtr<APlayerController> PlayerController;

	UFUNCTION(BlueprintCallable)
	float GetRotationSpeed() const { return RotationSpeed; }

	void SetAbilitySystemComponent(UAbilitySystemComponent* ASC) { AbilitySystemComponent = ASC; }
	
private:
	TObjectPtr<class UCharacterMovementComponent> OwnerMovementComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float RotationSpeed = 30.f;

	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
