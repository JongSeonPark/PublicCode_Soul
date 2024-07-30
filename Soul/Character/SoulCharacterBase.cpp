// Copyright 2024 ChickenGames All rights reserved.


#include "SoulCharacterBase.h"

#include "CharacterTrajectoryComponent.h"
#include "Soul.h"
#include "SoulGameplayTags.h"
#include "AbilitySystem/SoulGameplayAbilitySet.h"
#include "AbilitySystem/AttributeSet/SoulAttributeSet.h"
#include "Animation/SoulAnimInstance.h"
#include "Combat/CriticalHitAreaComponent.h"
#include "Combat/LockOn/LockOnPointComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Soul/AbilitySystem/SoulAbilitySystemComponent.h"
#include "UI/Widgets/SoulUserWidget.h"
#include "UI/WidgetControllers/SoulWorldHealthBarController.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/SoulEquipmentInventoryComponent.h"
#include "Inventory/SoulInventoryManagerComponent.h"

ASoulCharacterBase::ASoulCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
	
	CharacterTrajectory = CreateDefaultSubobject<UCharacterTrajectoryComponent>("Trajectory");
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidgetComponent");
	HealthBarWidgetComponent->SetDrawAtDesiredSize(true);
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;

	EquipmentInventoryComponent = CreateDefaultSubobject<USoulEquipmentInventoryComponent>("EquipmentInventoryComponent");
	InventoryManagerComponent = CreateDefaultSubobject<USoulInventoryManagerComponent>("InventoryManagerComponent");
}

UAbilitySystemComponent* ASoulCharacterBase::GetAbilitySystemComponent() const
{
	return SoulAbilitySystemComponent;
}

void ASoulCharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (!SoulAbilitySystemComponent) return;
	if (GetCharacterMovement()->MovementMode == MOVE_Falling) SoulAbilitySystemComponent->SetLooseGameplayTagCount(SoulGameplayTags::Status_Falling, 1);
	else SoulAbilitySystemComponent->SetLooseGameplayTagCount(SoulGameplayTags::Status_Falling, 0);
}

void ASoulCharacterBase::InitializeAbilitySystem()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ApplyStartupEffectsToSelf();
		AddCharacterAbilities();
	}
	
	SoulAbilitySystemComponent->RegisterGameplayTagEvent(SoulGameplayTags::Event_LockOnTargeted).AddLambda([this](const FGameplayTag Tag, int32 Count)
	{
		if (HealthBarController)
		{
			HealthBarController->OnTargeted.Broadcast(Count > 0);
		}
	});
	OnAbilitySystemInitialized.Broadcast(SoulAbilitySystemComponent);

	if (USoulAnimInstance* SoulAnimInst = Cast<USoulAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		SoulAnimInst->InitAbilitySystem(SoulAbilitySystemComponent);
	}

	SoulAbilitySystemComponent->RegisterGameplayTagEvent(SoulGameplayTags::Status_Dying).AddUObject(this, &ThisClass::OnDying);
	SoulAbilitySystemComponent->RegisterGameplayTagEvent(SoulGameplayTags::Status_Dead).AddUObject(this, &ThisClass::OnDead);

	TryCreateHealthBar();
}

void ASoulCharacterBase::ApplyStartupEffectsToSelf()
{
	if (!SoulAbilitySystemComponent) return;
	for (const TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		const FGameplayEffectContextHandle EffectContextHandle = SoulAbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = SoulAbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 0, EffectContextHandle);
		SoulAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void ASoulCharacterBase::AddCharacterAbilities()
{
	if (AbilitySet) AbilitySet->GiveAbilities(SoulAbilitySystemComponent);
}

void ASoulCharacterBase::OnDying(const FGameplayTag Tag, int32 Count)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	for (UActorComponent* Component : GetComponents())
	{
		if (Component->IsA<ULockOnPointComponent>())
		{
			Cast<ULockOnPointComponent>(Component)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		if (Component->IsA<UCriticalHitAreaComponent>())
		{
			Cast<UCriticalHitAreaComponent>(Component)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ASoulCharacterBase::OnDead(const FGameplayTag Tag, int32 Count)
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void ASoulCharacterBase::TryCreateHealthBar()
{
	if (HasAuthority())
	{
		// AI 제외
		if (GetRemoteRole() == ROLE_SimulatedProxy)
			CreateWorldHealthBarWidget();
		else
		{
			if (!IsLocallyControlled())
				CreateWorldHealthBarWidget();
		}
	}
	else
	{
		if (!IsLocallyControlled())
			CreateWorldHealthBarWidget();	
	}
}

void ASoulCharacterBase::CreateWorldHealthBarWidget()
{
	if (!HealthBarWidgetClass) return;
	if (HealthBarWidgetComponent->GetWidget()) return;
	
	HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetClass);
	HealthBarWidgetComponent->InitWidget();
	USoulUserWidget* SoulUserWidget = Cast<USoulUserWidget>(HealthBarWidgetComponent->GetWidget());
	HealthBarController = NewObject<USoulWorldHealthBarController>(this);
	HealthBarController->InitData(FControllerParams(this, SoulAbilitySystemComponent));
	SoulUserWidget->SetWidgetController(HealthBarController);
}
