// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_BaseEntity.h"
#include "BD_HealthAttributeSet.h"
#include "BD_AbilitySystemComponent.h"

// Sets default values
ABD_BaseEntity::ABD_BaseEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComp = CreateDefaultSubobject<UBD_AbilitySystemComponent>(TEXT("AbilitySystem"));
	HealthSet = CreateDefaultSubobject<UBD_HealthAttributeSet>(TEXT("Health"));
}

// Called when the game starts or when spawned
void ABD_BaseEntity::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComp->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* ABD_BaseEntity::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}