// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BD_BaseGameplayAbility.h"
#include "BD_GameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BLACKDESERT_API UBD_GameplayAbility_Jump : public UBD_BaseGameplayAbility
{
	GENERATED_BODY()

public:
	UBD_GameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	//virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};