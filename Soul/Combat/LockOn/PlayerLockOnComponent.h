// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LockOnComponent.h"
#include "PlayerLockOnComponent.generated.h"

struct FInputActionValue;

/**
 * LockOn Update를 실행하기 위한 컴포넌트 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_API UPlayerLockOnComponent : public ULockOnComponent
{
	GENERATED_BODY()

public:
	UPlayerLockOnComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetLockOnDelay();
	virtual void SetLockOnTarget(ULockOnPointComponent* InLockOnPoint);
	
	// 마땅한 타겟이 없을 때 앞을 바라보는 용도
	UFUNCTION(BlueprintCallable)
	void LookForward();

	ULockOnPointComponent* FindTarget();

private:
	UPROPERTY(EditDefaultsOnly, Category = "LockOn", meta = (AllowPrivateAccess))
	float CameraSpeed = 10.f;

	// 락온 했을 때, 카메라를 중앙에서 LockOnCameraOffset만큼 이동 시켜 고정한다.
	UPROPERTY(EditDefaultsOnly, Category = "LockOn", meta = (AllowPrivateAccess))
	FVector LockOnCameraOffset;

	// TODO: 카메라 전체 유닛을 체크할 수 있다면 삭제.
	// 한번에 찾는 Rect의 길이
	UPROPERTY(EditDefaultsOnly, Category = "LockOn", meta = (AllowPrivateAccess))
	FVector2D SearchRectArea;
	// 찾는 Rect의 깊이
	UPROPERTY(EditDefaultsOnly, Category = "LockOn", meta = (AllowPrivateAccess))
	float DirectionScaleFactor = 1000.f;

	bool bLookingForward = false;
	FVector LookForwardVector;

	void LookAtDirection(float DeltaTime, const FRotator& ForwardRotator) const;
	void LookForwardUpdate(float DeltaTime);

protected:
	virtual void LockOnUpdate(float DeltaTime) override;

public:
	// 다른 타겟으로 변경할 때 사용
	void UpdateLookInput(const FInputActionValue& Value);
	bool IsUpdateCamera() const { return bLookingForward || LockOnPoint != nullptr; }

private:
	void BoxTraceMulti_FindLockOnTarget(TArray<FHitResult>& HitResults);
	bool bLockOnDelay = false;

	TObjectPtr<class UWidgetComponent> PointWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "LockOn", meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> PointWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn", meta = (AllowPrivateAccess = true))
	bool bDrawDebugTrace = false;
	
};
