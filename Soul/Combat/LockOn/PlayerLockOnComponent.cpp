// Copyright 2024 ChickenGames All rights reserved.


#include "PlayerLockOnComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "InputActionValue.h"
#include "LockOnPointComponent.h"
#include "SoulGameplayTags.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UPlayerLockOnComponent::UPlayerLockOnComponent()
{
	LockOnCameraOffset = FVector(0.f, 0.f, 200.f);
	SearchRectArea = FVector2D(200.f, 200.f);
}

void UPlayerLockOnComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!OwnerCharacter->IsLocallyControlled()) return;
	PlayerController = CastChecked<APlayerController>(OwnerCharacter->Controller);
	PointWidgetComponent = NewObject<UWidgetComponent>(this->GetOwner(), UWidgetComponent::StaticClass(), FName("PointWidgetComponent"));
	PointWidgetComponent->SetWidgetClass(PointWidgetClass);
	PointWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PointWidgetComponent->SetVisibility(false);
	PointWidgetComponent->RegisterComponent();
}

void UPlayerLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!OwnerCharacter->IsLocallyControlled()) return;
	LookForwardUpdate(DeltaTime);
}

void UPlayerLockOnComponent::SetLockOnDelay()
{
	bLockOnDelay = true;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
	                                       [this]()
	                                       {
		                                       bLockOnDelay = false;
	                                       }, 1.f, false);
}

void UPlayerLockOnComponent::SetLockOnTarget(ULockOnPointComponent* InLockOnPoint)
{
	Super::SetLockOnTarget(InLockOnPoint);
	SetLockOnDelay();
	PointWidgetComponent->SetVisibility(InLockOnPoint != nullptr);

	if (const USceneComponent* OldAttachParent = PointWidgetComponent->GetAttachParent())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OldAttachParent->GetOwner()))
		{
			ASC->RemoveLooseGameplayTag(SoulGameplayTags::Event_LockOnTargeted);
		}
	}

	if (InLockOnPoint)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InLockOnPoint->GetOwner()))
		{
			ASC->AddLooseGameplayTag(SoulGameplayTags::Event_LockOnTargeted);
		}

		PointWidgetComponent->AttachToComponent(InLockOnPoint, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		PointWidgetComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	}
}

void UPlayerLockOnComponent::LookForward()
{
	if (bLookingForward) return;
	SetLockOnDelay();
	LookForwardVector = OwnerCharacter->GetActorForwardVector();
	bLookingForward = true;
}

void UPlayerLockOnComponent::BoxTraceMulti_FindLockOnTarget(TArray<FHitResult>& HitResults)
{
	TArray<AActor*> ActorToIgnores;
	FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	UKismetSystemLibrary::BoxTraceMulti(
		GetWorld(),
		CameraLocation,
		CameraLocation + CameraRotation.Vector() * DirectionScaleFactor,
		FVector(0, SearchRectArea.X, SearchRectArea.Y),
		PlayerController->PlayerCameraManager->GetCameraRotation(),
		UEngineTypes::ConvertToTraceType(ECC_LockOnPoint),
		false,
		ActorToIgnores,
		bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResults,
		false
	);

	// Matrix로 테스트했던 흔적
	// auto* ViewportClient = GetWorld()->GetGameViewport();
	// FSceneViewProjectionData ProjectionData;
	// PlayerController->GetLocalPlayer()->GetProjectionData(ViewportClient->Viewport, ProjectionData);
	// const FMatrix& ProjectionMatrix = ProjectionData.ProjectionMatrix;
	// FPlane NearPlane;
	// float Near = ProjectionData.GetNearPlaneFromProjectionMatrix();
	// ProjectionMatrix.GetFrustumNearPlane(NearPlane);
	// UE_LOG(LogTemp, Display, TEXT("ProjectionMatrix: %s"), *ProjectionMatrix.ToString());
	// UE_LOG(LogTemp, Display, TEXT("Near: %f, NearPlane: %s"), Near, *NearPlane.ToString());
}

ULockOnPointComponent* UPlayerLockOnComponent::FindTarget()
{
	// 내적은 -1 ~ 1이고, 1에 가까울수록(즉 커질수록) 적중률이 높아진다.
	// 거리는 0 ~ N까지고 가까울수록(작아질수록) 적중률이 높아진다.
	// 즉 내적을 Inverse 시켜서 1 ~ -1로 반전시킨게 그냥 -1 곱한거 아닌가?
	// 정규화된 방향백터와 포인트의 위치를 내적하면 거리근사값 * 내적.
	// 내적 음수값을 제외하겠음. (-면 화면에 안 보이고, 계산하려면 절대값으로 따로 해야함.)
	// 거리 100 내적 1 = 100
	// 거리 100 내적 0.2 = 20
	// 거리 1000 내적 1 = 1000
	// 거리 1000 내적 -1 = -1000
	// 거리 100 내적 -1 = -100

	// 내적 값에 -2 한다면?
	// 거리 100 내적 1 - 2(-1) = -100
	// 거리 100 내적 0.2 - 2(-1.8) = -180
	// 거리 1000 내적 1 - 2(-1) = -1000

	// 거리 1000 내적 -1 - 2(-3) = -3000
	// 거리 100 내적 -1 - 2(-3) = -300

	// -3
	// 거리 100 내적 1 - 3(-2) = -200
	// 거리 100 내적 0.2 - 3(-2.8) = -280
	// 거리 1000 내적 1 - 3(-2) = -2000
	// 포인트의 위치

	TArray<FHitResult> HitResults;
	BoxTraceMulti_FindLockOnTarget(HitResults);

	// TArray<TTuple<ULockOnPointComponent*, float>> PointAndFitPair;

	FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	FVector CameraDir = CameraRotation.Vector();

	float MaximumValue = -FLT_MAX;
	ULockOnPointComponent* SelectedPoint = nullptr;

	for (FHitResult Hit : HitResults)
	{
		if (Hit.Component->IsA(ULockOnPointComponent::StaticClass()))
		{
			ULockOnPointComponent* HitPoint = Cast<ULockOnPointComponent>(Hit.Component);
			// 상대 위치
			FVector Loc = (HitPoint->GetComponentLocation() - CameraLocation);
			FVector HitPointDir = Loc.ForwardVector;
			// -2 보정
			float DotValue = CameraDir.Dot(HitPointDir) - 2;
			float FitValue = DotValue * Loc.SizeSquared();

			if (FitValue > MaximumValue)
			{
				MaximumValue = FitValue;
				SelectedPoint = HitPoint;
			}
		}
	}

	return SelectedPoint;
}

void UPlayerLockOnComponent::LookAtDirection(float DeltaTime, const FRotator& ForwardRotator) const
{
	const FRotator ControlRot = PlayerController->GetControlRotation();
	const FRotator MoveDeltaRot = FMath::RInterpTo(ControlRot, ForwardRotator, DeltaTime, CameraSpeed);

	// UE_LOG(LogTemp, Display, TEXT("ForwardRot: %s, MoveDeltaRot: %s"), *ForwardRotator.ToString(), *MoveDeltaRot.ToString());
	PlayerController->SetControlRotation(MoveDeltaRot);
}

void UPlayerLockOnComponent::LookForwardUpdate(float DeltaTime)
{
	if (bLookingForward)
	{
		const FRotator ControlRot = PlayerController->GetControlRotation();
		FRotator ForwardRot = LookForwardVector.Rotation();
		ForwardRot.Pitch = ControlRot.Pitch;
		if ((ControlRot - ForwardRot).IsNearlyZero(DeltaTime * CameraSpeed))
		{
			PlayerController->SetControlRotation(ForwardRot);
			bLookingForward = false;
		}
		else
		{
			LookAtDirection(DeltaTime, ForwardRot);
		}
	}
}

void UPlayerLockOnComponent::LockOnUpdate(float DeltaTime)
{
	Super::LockOnUpdate(DeltaTime);
	if (!OwnerCharacter->IsLocallyControlled()) return;
	LookAtDirection(DeltaTime, (LockOnPoint->GetComponentLocation() - OwnerCharacter->GetActorLocation() - LockOnCameraOffset).Rotation());

	// 상대가 죽었을 때 락온 취소
	if (LockOnPoint)
	{
		bool bDead = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(LockOnPoint->GetOwner())->HasAnyMatchingGameplayTags(
			FGameplayTagContainer::CreateFromArray(
				TArray<FGameplayTag>(
					{
						SoulGameplayTags::Status_Dead,
						SoulGameplayTags::Status_Dying
					})));
		if (bDead)
		{
			SetLockOnTarget(FindTarget());
		}
	}
}

void UPlayerLockOnComponent::UpdateLookInput(const FInputActionValue& Value)
{
	if (!LockOnPoint) return;
	if (bLockOnDelay) return;
	static const float UpdateThreshold = 1.f;

	// UE_LOG(LogTemp, Display, TEXT("Input Value: %s, MagnitudeSq: %f"), *Value.ToString(), Value.GetMagnitudeSq());
	if (Value.GetMagnitudeSq() > UpdateThreshold)
	{
		FVector2D InputDir = Value.Get<FVector2D>().GetSafeNormal();
		TArray<FHitResult> HitResults;
		BoxTraceMulti_FindLockOnTarget(HitResults);

		FVector2D CurrentTargetScreenPosition;
		UGameplayStatics::ProjectWorldToScreen(PlayerController, LockOnPoint->GetComponentLocation(), CurrentTargetScreenPosition);

		// FInputActionValue가 주는 방향 벡터와의 적합도가 가장 높은 걸로 바꾸면 되겠지.
		float SelectedValue = FLT_MIN;
		ULockOnPointComponent* SelectedPoint = nullptr;

		for (FHitResult HitResult : HitResults)
		{
			if (HitResult.Component->IsA(ULockOnPointComponent::StaticClass()))
			{
				ULockOnPointComponent* HitPoint = Cast<ULockOnPointComponent>(HitResult.Component);
				if (HitPoint == LockOnPoint) continue;

				FVector2D ScreenPosition;
				UGameplayStatics::ProjectWorldToScreen(PlayerController, HitPoint->GetComponentLocation(), ScreenPosition);

				FVector2D Dir = (ScreenPosition - CurrentTargetScreenPosition).GetSafeNormal();
				float Dot = InputDir.Dot(Dir);
				// UE_LOG(LogTemp, Display, TEXT("Target: %s, Dir: %s, Dot: %f"), *HitPoint->GetName(), *Dir.ToString(), Dot);

				if (Dot > SelectedValue)
				{
					SelectedPoint = HitPoint;
					SelectedValue = Dot;
				}
			}
		}

		if (SelectedPoint != nullptr)
		{
			SetLockOnTarget(SelectedPoint);
		}
	}
}
