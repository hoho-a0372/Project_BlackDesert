// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BD_HealthAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeChangedEvent, UAttributeSet*, AttributeSet, float, OldValue, float, NewValue);

UCLASS()
class PROJECT_BLACKDESERT_API UBD_HealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	UBD_HealthAttributeSet();

	ATTRIBUTE_ACCESSORS_BASIC(UBD_HealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UBD_HealthAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UBD_HealthAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (HideFromModifiers))
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedEvent OnHealthChanged;

	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Damage;
};
