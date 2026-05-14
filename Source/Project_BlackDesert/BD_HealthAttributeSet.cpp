// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_HealthAttributeSet.h"
#include "GameplayEffectExtension.h"

UBD_HealthAttributeSet::UBD_HealthAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
}

void UBD_HealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("PreChange: Attribute '%s'"), *Attribute.AttributeName);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	Super::PreAttributeChange(Attribute, NewValue);
}

void UBD_HealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	UE_LOG(LogTemp, Warning, TEXT("PostChange: Attribute '%s' changed %.2f -> %.2f"), *Attribute.AttributeName, OldValue, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(this, OldValue, NewValue);
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// When max health changes, broadcast OnHealthChanged so that health bars will update
		const float CurrentHealth = GetHealth();
		OnHealthChanged.Broadcast(this, CurrentHealth, CurrentHealth);
	}
}

void UBD_HealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Convert into -Health and then clamp
		const float DamageValue = GetDamage();
		const float OldHealthValue = GetHealth();
		const float MaxHealthValue = GetMaxHealth();
		const float NewHealthValue = FMath::Clamp(OldHealthValue - DamageValue, 0.0f, MaxHealthValue);

		if (OldHealthValue != NewHealthValue)
		{
			// Set the new health after clamping to min-max
			SetHealth(NewHealthValue);
		}

		// Clear the meta attribute that temporarily held damage
		SetDamage(0.0f);
	}
}

//void UBD_HealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
//{
//	GAMEPLAYATTRIBUTE_REPNOTIFY(UBD_HealthAttributeSet, Health, OldValue);
//	const float OldHealth = OldValue.GetCurrentValue();
//	const float NewHealth = GetHealth();
//	OnHealthChanged.Broadcast(this, OldHealth, NewHealth);
//}
//
//void UBD_HealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
//{
//	GAMEPLAYATTRIBUTE_REPNOTIFY(UBD_HealthAttributeSet, MaxHealth, OldValue);
//
//	// When max health changes, broadcast OnHealthChanged so that health bars will update
//	const float CurrentHealth = GetHealth();
//	OnHealthChanged.Broadcast(this, CurrentHealth, CurrentHealth);
//}