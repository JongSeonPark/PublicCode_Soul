// Copyright 2024 ChickenGames All rights reserved.


#include "SoulPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Soul.h"
#include "SoulGameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/SoulGameplayAbilitySet.h"
#include "AbilitySystem/AttributeSet/SoulAttributeSet.h"
#include "Combat/LockOn/PlayerLockOnComponent.h"
#include "Components/BoxComponent.h"
#include "Game/SoulGameInstance.h"
#include "InteractiveObjects/Interaction_Door.h"
#include "InteractiveObjects/InteractiveObjectInterface.h"
#include "Player/SoulPlayerState.h"
#include "Save/SoulSaveGame.h"
#include "UI/SoulUISubsystem.h"
#include "UI/WidgetControllers/SoulIngameWidgetController.h"
#include "UserSettings/EnhancedInputUserSettings.h"

class AInteraction_Door;

ASoulPlayerCharacter::ASoulPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	LockOnComponent = CreateDefaultSubobject<UPlayerLockOnComponent>(TEXT("PlayerLockOnComponent"));
	InteractionAreaComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBoxComponent"));
	InteractionAreaComponent->InitBoxExtent(FVector(32.f, 32.f, 96.f));
	InteractionAreaComponent->SetRelativeLocation(FVector(50.f, 0.f, 0.f));
	InteractionAreaComponent->SetupAttachment(RootComponent);
}

void ASoulPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InteractionAreaComponent->OnComponentBeginOverlap.AddDynamic(this, &ASoulPlayerCharacter::OnInteractionBeginOverlap);
	InteractionAreaComponent->OnComponentEndOverlap.AddDynamic(this, &ASoulPlayerCharacter::OnInteractionEndOverlap);

	if (USoulGameInstance* SoulGameInstance = USoulGameplayStatics::GetSoulGameInstance(GetWorld()))
	{
		SoulGameInstance->OnSoulSave.AddDynamic(this, &ASoulPlayerCharacter::OnSaveGame);
		if (USoulSaveGame* SoulSaveGame = SoulGameInstance->SoulSaveGame)
		{
			if (!SoulSaveGame->DoorKey.IsNone())
			{
				UWorld* World = GetWorld();
				for (TActorIterator<AInteraction_Door> It(World); It; ++It)
				{
					AInteraction_Door* Door = *It;
					if (SoulSaveGame->DoorKey.IsEqual(Door->GetDoorKey()))
					{
						SetActorTransform(Door->GetTravelTransform());
						return;
					}
				}
			}
			if (!SoulSaveGame->LastTransform.Equals(FTransform::Identity))
			{
				SetActorTransform(SoulSaveGame->LastTransform);
			}
		}
	}
}

void ASoulPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
	// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
	InitializeAbilitySystem();
}

void ASoulPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Set the ASC for clients. Server does this in PossessedBy.
	// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
	InitializeAbilitySystem();
}

void ASoulPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (AbilitySet) AbilitySet->BindInputs(PlayerInputComponent, SoulAbilitySystemComponent);
	APlayerController* PlayerController = GetController<APlayerController>();
	if (!PlayerController) return;

	// Mapping 지정하기
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		UEnhancedInputUserSettings* UserSetting = Subsystem->GetUserSettings();
		if (!UserSetting->IsMappingContextRegistered(DefaultMappingContext))
		{
			UserSetting->RegisterInputMappingContext(DefaultMappingContext);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASoulPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoulPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::LockOn);
	}
}

void ASoulPlayerCharacter::InitializeAbilitySystem()
{
	ASoulPlayerState* PS = GetPlayerState<ASoulPlayerState>();
	if (PS)
	{
		SoulAbilitySystemComponent = Cast<USoulAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		SoulAbilitySystemComponent->InitAbilityActorInfo(PS, this);
		SoulAttributeSet = PS->GetSoulAttributeSet();
	}
	
	if (AbilitySet) SoulAbilitySystemComponent->SetBindInputToActivateTags(AbilitySet->BindInputToActivateTags);

	LockOnComponent->SetAbilitySystemComponent(SoulAbilitySystemComponent);

	if (IsLocallyControlled())
	{
		USoulUISubsystem* UISubsystem = GetWorld()->GetSubsystem<USoulUISubsystem>();
		USoulIngameWidgetController* WidgetController = UISubsystem->GetOrAddWidgetController<USoulIngameWidgetController>();
		WidgetController->InitData(FControllerParams(this, SoulAbilitySystemComponent));
	}
	Super::InitializeAbilitySystem();
}

void ASoulPlayerCharacter::ApplyStartupEffectsToSelf()
{
	Super::ApplyStartupEffectsToSelf();
	// Effect가 적용된 이후 Attribute 적용
	if (USoulGameInstance* SoulGameInstance = USoulGameplayStatics::GetSoulGameInstance(GetWorld()))
	{
		if (USoulSaveGame* SaveGame = SoulGameInstance->SoulSaveGame)
		{
			if (!SaveGame->bInitGame)
			{
				SoulAttributeSet->SetHealth(SaveGame->Health);
				SoulAttributeSet->SetStanceHealth(SaveGame->StanceHealth);
				SoulAttributeSet->SetPoiseHealth(SaveGame->PoiseHealth);
				SoulAttributeSet->SetMana(SaveGame->Mana);
				SoulAttributeSet->SetStamina(SaveGame->Stamina);
			}
			else
			{
				SaveGame->bInitGame = false;
			}
		}
	}
}

void ASoulPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASoulPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	LockOnComponent->UpdateLookInput(Value);
	if (!LockOnComponent->IsUpdateCamera())
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASoulPlayerCharacter::LockOn()
{
	if (LockOnComponent->GetLockOnTarget())
	{
		LockOnComponent->SetLockOnTarget(nullptr);
	}
	else
	{
		if (ULockOnPointComponent* Target = LockOnComponent->FindTarget())
		{
			LockOnComponent->SetLockOnTarget(Target);
		}
		else
		{
			// 못 찾았으면 앞을 본다.
			LockOnComponent->LookForward();
		}
	}
}

void ASoulPlayerCharacter::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (Other->Implements<UInteractiveObjectInterface>())
	{
		InteractionActorsInArea.Add(Other);
	}
}

void ASoulPlayerCharacter::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Other->Implements<UInteractiveObjectInterface>())
	{
		InteractionActorsInArea.Remove(Other);
	}
}

void ASoulPlayerCharacter::OnSaveGame(USoulSaveGame* SaveGame)
{
	SaveGame->LastTransform = GetActorTransform();
	
	SaveGame->Health = SoulAttributeSet->GetHealth();
	SaveGame->StanceHealth = SoulAttributeSet->GetStanceHealth();
	SaveGame->PoiseHealth = SoulAttributeSet->GetPoiseHealth();
	SaveGame->Mana = SoulAttributeSet->GetMana();
	SaveGame->Stamina = SoulAttributeSet->GetStamina();
}
