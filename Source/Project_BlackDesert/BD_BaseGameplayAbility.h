// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BD_BaseGameplayAbility.generated.h"

class UBD_AbilitySystemComponent;
class ABD_BaseCharacter;
class ABD_BaseCharacter;
class AController;

UENUM(BlueprintType)
enum class EBD_AbilityActivationPolicy : uint8
{
	OnInputTriggered, // 입력 시
	WhileInputActive, // 입력하는 동안
	OnSpawn			  // 스폰 시, ex. 패시브 스킬
};

UENUM(BlueprintType)
enum class EBD_AbilityActivationGroup : uint8
{
	Independent,           // 다른 스킬과 무관하게 독립적으로 실행 (예: 쿨타임 없는 버프)
	Exclusive_Replaceable, // 실행 중 다른 스킬에 의해 캔슬될 수 있음 (예: 평타 도중 회피기 사용)
	Exclusive_Blocking,    // 실행 중 다른 스킬의 사용을 막음 (예: 강력한 캐스팅 스킬, 피격 모션)
	MAX UMETA(Hidden)
};

UCLASS(Abstract)
class PROJECT_BLACKDESERT_API UBD_BaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBD_BaseGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Ability|BDGAS")
	UBD_AbilitySystemComponent* GetBDAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Ability|BDGAS")
	ABD_BaseCharacter* GetBDCharacterFromActorInfo() const;

	// Get AIController and PlayerController
	UFUNCTION(BlueprintCallable, Category = "Ability|BDGAS")
	AController* GetControllerFromActorInfo() const;

	EBD_AbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	EBD_AbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ability|BDGAS")
	bool CanChangeActivationGroup(EBD_AbilityActivationGroup NewGroup) const;

	// Tries to change the activation group(Replaceable to Blocking).  Returns true if it successfully changed.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ability|BDGAS")
	bool ChangeActivationGroup(EBD_AbilityActivationGroup NewGroup);

	// activate passive skill on spawn
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	virtual void OnPawnAvatarSet();

	// Called when the ability fails to activate
	virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability", DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();

protected:

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation|BDGAS")
	EBD_AbilityActivationPolicy ActivationPolicy;

	// Defines the relationship between this ability activating and other abilities activating.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation|BDGAS")
	EBD_AbilityActivationGroup ActivationGroup;

};
