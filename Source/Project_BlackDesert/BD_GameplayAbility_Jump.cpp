// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_GameplayAbility_Jump.h"
#include "GameFramework/Character.h"
#include "BD_BaseGameplayAbility.h"

UBD_GameplayAbility_Jump::UBD_GameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 현재 GA(Jump)를 식별하는 태그
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Action.Jump")));
	// 현재 GA를 실행하는 동안 캐릭터(플레이어)에게 부여되는 태그
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Movement.Jumping")));
}

void UBD_GameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// cost 계산, 쿨타임 체크는 super의 CommitAbility()를 통해 수행
	// Character->Jump()
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 사운드, 이펙트 추가
}

void UBD_GameplayAbility_Jump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	// Character->StopJumping()
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
