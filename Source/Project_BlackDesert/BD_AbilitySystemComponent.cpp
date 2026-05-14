// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_AbilitySystemComponent.h"
#include "BD_BaseGameplayAbility.h"
#include "GameFramework/Pawn.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

UBD_AbilitySystemComponent::UBD_AbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();

	FMemory::Memset(ActivationGroupCounts, 0, sizeof(ActivationGroupCounts));
}

void UBD_AbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
			for (auto AbilityInstance : Instances)
			{
				if (UBD_BaseGameplayAbility* BDAbilityInstance = Cast<UBD_BaseGameplayAbility>(AbilityInstance))
				{
					BDAbilityInstance->OnPawnAvatarSet();
				}
			}
		}

		// initialize anim instance

		// OnSpawn Policy에 해당하는 Ability 활성화
		TryActivateAbilitiesOnSpawn();
	}

}

void UBD_AbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	// ability list 변화 방지 lock (ActivatableAbilities를 순회하는 경우 반드시 사용. {} scope내에서 작동)
	// giveAbility 혹은 clearAbility가 호출되면 queue에 예약
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (const UBD_BaseGameplayAbility* BDAbility = Cast<UBD_BaseGameplayAbility>(AbilitySpec.Ability))
		{
			BDAbility->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
		}
	}
}

void UBD_AbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc)
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.IsActive())
		{
			continue;
		}

		TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
		for (auto AbilityInstance : Instances)
		{
			if (UBD_BaseGameplayAbility* BDAbilityInstance = Cast<UBD_BaseGameplayAbility>(AbilityInstance))
			{
				if (ShouldCancelFunc(BDAbilityInstance, AbilitySpec.Handle))
				{
					if (BDAbilityInstance->CanBeCanceled())
					{
						BDAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), BDAbilityInstance->GetCurrentActivationInfo(), false);
					}
				}
			}
		}

	}
}

void UBD_AbilitySystemComponent::CancelInputActivatedAbilities()
{
	auto ShouldCancelFunc = [this](const UBD_BaseGameplayAbility* Ability, FGameplayAbilitySpecHandle Handle)
	{
		const EBD_AbilityActivationPolicy ActivationPolicy = Ability->GetActivationPolicy();
		return ((ActivationPolicy == EBD_AbilityActivationPolicy::OnInputTriggered) || (ActivationPolicy == EBD_AbilityActivationPolicy::WhileInputActive));
	};
	CancelAbilitiesByFunc(ShouldCancelFunc);
}


void UBD_AbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

}

void UBD_AbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);
}

void UBD_AbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UBD_AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UBD_AbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	// Process all abilities that activate when the input is held.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UBD_BaseGameplayAbility* Ability = Cast<UBD_BaseGameplayAbility>(AbilitySpec->Ability);
				if (Ability && Ability->GetActivationPolicy() == EBD_AbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(SpecHandle);
				}
			}
		}
	}

	// Process all abilities that had their input pressed this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability) 
			{
				AbilitySpec->InputPressed = true;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else 
				{
					const UBD_BaseGameplayAbility* Ability = Cast<UBD_BaseGameplayAbility>(AbilitySpec->Ability);
					if (Ability && Ability->GetActivationPolicy() == EBD_AbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(SpecHandle);
					}
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(SpecHandle);
	}

	// Process all abilities that had their input released this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && AbilitySpec->IsActive())
			{
				AbilitySpecInputReleased(*AbilitySpec);
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UBD_AbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UBD_AbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);

	if (UBD_BaseGameplayAbility* BDAbility = Cast<UBD_BaseGameplayAbility>(Ability))
	{
		AddAbilityToActivationGroup(BDAbility->GetActivationGroup(), BDAbility);
	}
}

void UBD_AbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);

	HandleAbilityFailed(Ability, FailureReason);
}

void UBD_AbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	if (UBD_BaseGameplayAbility* BDAbility = Cast<UBD_BaseGameplayAbility>(Ability))
	{
		RemoveAbilityFromActivationGroup(BDAbility->GetActivationGroup(), BDAbility);
	}
}

void UBD_AbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	if (const UBD_BaseGameplayAbility* BDAbility = Cast<const UBD_BaseGameplayAbility>(Ability))
	{
		BDAbility->NativeOnAbilityFailedToActivate(FailureReason);
	}
}

bool UBD_AbilitySystemComponent::IsActivationGroupBlocked(EBD_AbilityActivationGroup Group) const
{
	bool bBlocked = false;

	switch (Group)
	{
	case EBD_AbilityActivationGroup::Independent:
		bBlocked = false;
		break;
	case EBD_AbilityActivationGroup::Exclusive_Replaceable:
	case EBD_AbilityActivationGroup::Exclusive_Blocking:
		bBlocked = (ActivationGroupCounts[(uint8)EBD_AbilityActivationGroup::Exclusive_Blocking] > 0);
		break;
	default:
		break;
	}

	return bBlocked;
}

void UBD_AbilitySystemComponent::AddAbilityToActivationGroup(EBD_AbilityActivationGroup Group, UBD_BaseGameplayAbility* Ability)
{
	ActivationGroupCounts[(uint8)Group]++;

	switch (Group)
	{
	case EBD_AbilityActivationGroup::Independent:
		break;
	case EBD_AbilityActivationGroup::Exclusive_Replaceable:
	case EBD_AbilityActivationGroup::Exclusive_Blocking:
		// 다른 Replaceable 스킬 강제로 취소하기
		CancelActivationGroupAbilities(EBD_AbilityActivationGroup::Exclusive_Replaceable, Ability);
	default:
		break;
	}
}

void UBD_AbilitySystemComponent::RemoveAbilityFromActivationGroup(EBD_AbilityActivationGroup Group, UBD_BaseGameplayAbility* Ability)
{
	if (ActivationGroupCounts[(uint8)Group] > 0)
	{
		ActivationGroupCounts[(uint8)Group]--;
	}
}

void UBD_AbilitySystemComponent::CancelActivationGroupAbilities(EBD_AbilityActivationGroup Group, UBD_BaseGameplayAbility* IgnoreAbility)
{
	auto ShouldCancelFunc = [Group, IgnoreAbility](const UBD_BaseGameplayAbility* Ability, FGameplayAbilitySpecHandle Handle)
	{
		return ((Ability->GetActivationGroup() == Group) && (Ability != IgnoreAbility));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc);
}
