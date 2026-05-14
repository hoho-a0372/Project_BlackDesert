// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_BaseGameplayAbility.h"
#include "BD_AbilitySystemComponent.h"
#include "BD_BaseCharacter.h"

UBD_BaseGameplayAbility::UBD_BaseGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	ActivationPolicy = EBD_AbilityActivationPolicy::OnInputTriggered;
	ActivationGroup = EBD_AbilityActivationGroup::Independent;
}

UBD_AbilitySystemComponent* UBD_BaseGameplayAbility::GetBDAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UBD_AbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ABD_BaseCharacter* UBD_BaseGameplayAbility::GetBDCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ABD_BaseCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

AController* UBD_BaseGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		if (APawn* AvatarPawn = Cast<APawn>(CurrentActorInfo->AvatarActor.Get()))
		{
			return AvatarPawn->GetController();
		}
	}
	return nullptr;
}

bool UBD_BaseGameplayAbility::CanChangeActivationGroup(EBD_AbilityActivationGroup NewGroup) const
{
	if (!IsInstantiated() || !IsActive())
	{
		return false;
	}

	if (ActivationGroup == NewGroup)
	{
		return true;
	}

	if (UBD_AbilitySystemComponent* ASC = GetBDAbilitySystemComponentFromActorInfo())
	{
		if ((ActivationGroup != EBD_AbilityActivationGroup::Exclusive_Blocking) && ASC->IsActivationGroupBlocked(NewGroup))
		{
			return false;
		}

		if ((NewGroup == EBD_AbilityActivationGroup::Exclusive_Replaceable) && !CanBeCanceled())
		{
			return false;
		}
	}
	return true;
}

bool UBD_BaseGameplayAbility::ChangeActivationGroup(EBD_AbilityActivationGroup NewGroup)
{
	return false;
}

void UBD_BaseGameplayAbility::NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
{
}

bool UBD_BaseGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return false;
}

void UBD_BaseGameplayAbility::SetCanBeCanceled(bool bCanBeCanceled)
{
}

void UBD_BaseGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
}

void UBD_BaseGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
}

void UBD_BaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
}

void UBD_BaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}

void UBD_BaseGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && (ActivationPolicy == EBD_AbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		if (ASC && AvatarActor && !AvatarActor->IsPendingKillPending() && (AvatarActor->GetLifeSpan() <= 0.f))
		{
			ASC->TryActivateAbility(Spec.Handle);
		}
	}
}

void UBD_BaseGameplayAbility::OnPawnAvatarSet()
{
	K2_OnPawnAvatarSet();
}
