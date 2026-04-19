// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BD_HealthAttributeSet.generated.h"

UCLASS()
class PROJECT_BLACKDESERT_API UBD_HealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	UBD_HealthAttributeSet();

	ATTRIBUTE_ACCESSORS_BASIC(UBD_HealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UBD_HealthAttributeSet, MaxHealth);
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	FGameplayAttributeData MaxHealth;
};
