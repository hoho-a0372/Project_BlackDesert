// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_PlayerState.h"
#include "BD_AbilitySystemComponent.h"
#include "BD_PlayerController.h"
#include "BD_BaseCharacter.h"
#include "BD_PlayerClassData.h"
#include "BD_HealthAttributeSet.h"
#include "BD_CombatAttributeSet.h"

ABD_PlayerState::ABD_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBD_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HealthSet = CreateDefaultSubobject<UBD_HealthAttributeSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UBD_CombatAttributeSet>(TEXT("CombatSet"));
}

UAbilitySystemComponent* ABD_PlayerState::GetAbilitySystemComponent() const
{
	return GetBDAbilitySystemComponent();
}

void ABD_PlayerState::SetPlayerData(const UBD_PlayerClassData* InPlayerData)
{
	check(InPlayerData);

	if (PlayerData)
	{
		//UE_LOG(LogTemp, Error, TEXT("Trying to set PlayerData [%s] on player state [%s] that already has valid PlayerData [%s]."), *GetNameSafe(InPlayerData), *GetNameSafe(this), *GetNameSafe(PlayerData));
		return;
	}

	PlayerData = InPlayerData;

	// Assign Abilities from PlayerData to PlayerState

}

ABD_PlayerController* ABD_PlayerState::GetBDPlayerController() const
{
	return Cast<ABD_PlayerController>(GetOwner());
}

void ABD_PlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ABD_PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn()); // owner: playerstate, avatar: playerCharacter
}

