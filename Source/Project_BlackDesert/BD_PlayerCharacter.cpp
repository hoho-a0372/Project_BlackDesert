// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_PlayerCharacter.h"
#include "BD_HealthBar.h"
#include "BD_AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"

ABD_PlayerCharacter::ABD_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FQuat(FRotator(0.f, -90.f, 0.f)));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultCameraDistance;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw =false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->JumpZVelocity = 500.f;

	//HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	//HealthBar->SetupAttachment(FollowCamera);
}

void ABD_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext) {
				InputSystem->AddMappingContext(DefaultMappingContext, 0);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Input Mapping Context is NULL!"));
			}
		}
	}
	//HealthBarWidget = Cast<UBD_HealthBar>(HealthBar->GetUserWidgetObject());
	//check(HealthBarWidget);
}

void ABD_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ABD_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedComp = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABD_PlayerCharacter::Move);
		EnhancedComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABD_PlayerCharacter::Look);
		EnhancedComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABD_PlayerCharacter::JumpInputPressed);
		EnhancedComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABD_PlayerCharacter::JumpInputReleased);
		EnhancedComp->BindAction(SprintAction, ETriggerEvent::Started, this, &ABD_PlayerCharacter::SprintStart);
		EnhancedComp->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABD_PlayerCharacter::SprintEnd);
	}
}

void ABD_PlayerCharacter::Move(const FInputActionValue& Value)
{
	const auto MoveVector = Value.Get<FVector2D>();
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Black, FString::Printf(TEXT("Move : %s"), *MoveVector.ToString()));
	if (GetController())
	{
		const auto Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MoveVector.Y);
		AddMovementInput(RightDirection, MoveVector.X);
	}
}

void ABD_PlayerCharacter::Look(const FInputActionValue& Value)
{
	const auto LookAxisVector = Value.Get<FVector2D>();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Black, FString::Printf(TEXT("Look : %s"), *LookAxisVector.ToString()));

	if (GetController()) 
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABD_PlayerCharacter::JumpInputPressed()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent()) 
	{
		FGameplayTagContainer JumpTags;
		JumpTags.AddTag(JumpInputTag);
		ASC->TryActivateAbilitiesByTag(JumpTags);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Black, FString::Printf(TEXT("Jump Pressed and Add Tag")));

		for (auto& Spec : ASC->GetActivatableAbilities())
		{
			if (Spec.Ability && Spec.DynamicAbilityTags.HasTagExact(JumpInputTag))
			{
				ASC->AbilitySpecInputPressed(Spec);
			}
		}
	}
}

void ABD_PlayerCharacter::JumpInputReleased()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (auto& Spec : ASC->GetActivatableAbilities())
		{
			// JumpInputTag를 가진 어빌리티에서 종료 (ex. GA_Jump::EndAbility)
			if (Spec.Ability && Spec.DynamicAbilityTags.HasTagExact(JumpInputTag))
			{
				ASC->AbilitySpecInputReleased(Spec);
			}
		}
	}
}
void ABD_PlayerCharacter::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
}

void ABD_PlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}
