// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_BaseCharacter.h"
#include "BD_PlayerState.h"
#include "BD_PlayerController.h"
#include "BD_HealthAttributeSet.h"
#include "BD_CombatAttributeSet.h"
#include "BD_AbilitySystemComponent.h"

// Sets default values
ABD_BaseCharacter::ABD_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//AbilitySystemComp = CreateDefaultSubobject<UBD_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//HealthSet = CreateDefaultSubobject<UBD_HealthAttributeSet>(TEXT("HealthSet"));
	//CombatSet = CreateDefaultSubobject<UBD_CombatAttributeSet>(TEXT("CombatSet"));

	AbilitySystemComponent = nullptr;
}

ABD_PlayerController* ABD_BaseCharacter::GetBDPlayerController() const
{
	return CastChecked<ABD_PlayerController>(GetController(), ECastCheckedType::NullAllowed);
}

ABD_PlayerState* ABD_BaseCharacter::GetBDPlayerState() const
{
	return CastChecked<ABD_PlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UAbilitySystemComponent* ABD_BaseCharacter::GetAbilitySystemComponent() const
{
	return GetBDAbilitySystemComponent();
}

// Called when the game starts or when spawned
void ABD_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ABD_BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();
}

void ABD_BaseCharacter::InitializeGameplayTags()
{
}


void ABD_BaseCharacter::InitializeAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		UnInitializeAbilitySystem();
	}

	if (ABD_PlayerState* PS = GetBDPlayerState())
	{
		if (AbilitySystemComponent = PS->GetBDAbilitySystemComponent())
		{
			// ASC initializer => Owner: PlayerState, Avatar: Character
			AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		}
		// health initialize
		InitializeGameplayTags();
		OnAbilitySystemInitialized.Broadcast();
	}
}

void ABD_BaseCharacter::UnInitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{

		// cancel and clear abilities

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}
		OnAbilitySystemUninitialized.Broadcast();
	}
	AbilitySystemComponent = nullptr;
}
