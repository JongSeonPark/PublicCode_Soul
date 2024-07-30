// Copyright 2024 ChickenGames All rights reserved.


#include "LockOnComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "LockOnPointComponent.h"
#include "SoulGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ULockOnComponent::ULockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	OwnerMovementComponent = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
}

void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!OwnerCharacter || !OwnerMovementComponent || !LockOnPoint) return;
	LockOnUpdate(DeltaTime);
}

void ULockOnComponent::SetLockOnTarget(ULockOnPointComponent* InLockOnPoint)
{
	LockOnPoint = InLockOnPoint;
	OwnerMovementComponent->bOrientRotationToMovement = LockOnPoint == nullptr;
	SetLockOnTarget_Server(InLockOnPoint);

	if (AbilitySystemComponent)
		AbilitySystemComponent->SetLooseGameplayTagCount(SoulGameplayTags::Status_Action_LockOn, LockOnPoint == nullptr ? 0 : 1);
}

void ULockOnComponent::SetLockOnTarget_Server_Implementation(ULockOnPointComponent* InLockOnPoint)
{
	LockOnPoint = InLockOnPoint;
	OwnerMovementComponent->bOrientRotationToMovement = LockOnPoint == nullptr;
}

void ULockOnComponent::LockOnUpdate(float DeltaTime)
{
	if (AbilitySystemComponent && !AbilitySystemComponent->HasMatchingGameplayTag(SoulGameplayTags::Status_Force_LockOnUpdate))
	{
		static const FGameplayTagContainer MatchingTagContainer = FGameplayTagContainer::CreateFromArray(
			TArray<FGameplayTag>({
				SoulGameplayTags::Status_Attack,
				SoulGameplayTags::Status_Action_Rolling,
				SoulGameplayTags::Status_Movement_Sprinting
			}));
		OwnerMovementComponent->bOrientRotationToMovement = AbilitySystemComponent->HasMatchingGameplayTag(SoulGameplayTags::Status_Movement_Sprinting);
		if (AbilitySystemComponent->HasAnyMatchingGameplayTags(MatchingTagContainer))
			return;
	}
	const FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	const FVector TargetLocation = LockOnPoint->GetComponentLocation();

	const FRotator OwnerRot = OwnerCharacter->GetActorRotation();
	const FRotator DesireRot = (TargetLocation - OwnerLocation).GetSafeNormal2D().Rotation();
	FRotator InterpTo = FMath::RInterpTo(OwnerRot, DesireRot, DeltaTime, RotationSpeed);

	OwnerCharacter->SetActorRotation(InterpTo);
}
