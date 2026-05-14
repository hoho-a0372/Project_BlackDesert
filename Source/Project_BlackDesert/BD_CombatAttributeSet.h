// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BD_CombatAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BLACKDESERT_API UBD_CombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	UBD_CombatAttributeSet();

	ATTRIBUTE_ACCESSORS_BASIC(UBD_CombatAttributeSet, AttackPower);
	ATTRIBUTE_ACCESSORS_BASIC(UBD_CombatAttributeSet, DefensePower);
	ATTRIBUTE_ACCESSORS_BASIC(UBD_CombatAttributeSet, CriticalChance);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	FGameplayAttributeData AttackPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	FGameplayAttributeData DefensePower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	FGameplayAttributeData CriticalChance;
};
