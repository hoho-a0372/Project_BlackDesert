// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "BD_BaseGameplayAbility.h"

#include "BD_AbilitySystemComponent.generated.h"

class UGameplayAbility;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);

UCLASS()
class PROJECT_BLACKDESERT_API UBD_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UBD_AbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	typedef TFunctionRef<bool(const UBD_BaseGameplayAbility* Ability, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc);
	void CancelInputActivatedAbilities();

	// inputTag 기반 ability 상태 처리 (Pressed, Released, Held)
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	// Cancel or block skill based on Activation Group
	bool IsActivationGroupBlocked(EBD_AbilityActivationGroup Group) const;
	void AddAbilityToActivationGroup(EBD_AbilityActivationGroup Group, UBD_BaseGameplayAbility* Ability);
	void RemoveAbilityFromActivationGroup(EBD_AbilityActivationGroup Group, UBD_BaseGameplayAbility* Ability);
	void CancelActivationGroupAbilities(EBD_AbilityActivationGroup Group, UBD_BaseGameplayAbility* IgnoreAbility);

	void TryActivateAbilitiesOnSpawn();

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;

	void HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);


protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	// Number of abilities running in each activation group.
	int32 ActivationGroupCounts[(uint8)EBD_AbilityActivationGroup::MAX];
};
