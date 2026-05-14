// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BD_BaseCharacter.generated.h"

class ABD_PlayerState;
class ABD_PlayerController;
class UBD_AbilitySystemComponent;

UCLASS(Abstract)
class PROJECT_BLACKDESERT_API ABD_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ABD_BaseCharacter();

	UFUNCTION(BlueprintCallable, Category = "Character")
	ABD_PlayerController* GetBDPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	ABD_PlayerState* GetBDPlayerState() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GAS|Character")
	UBD_AbilitySystemComponent* GetBDAbilitySystemComponent() const { return AbilitySystemComponent; };


	void InitializeAbilitySystem();

	void UnInitializeAbilitySystem();



protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	void InitializeGameplayTags();

	//void  Die();
	//virtual void InitalizeAttributes();


protected:
	// Pointer to the ability system component that is cached for convenience.
	// Transient : 디스크로부터 데이터 로딩 방지, 데이터 저장X (외부 컴포넌트 캐싱)
	UPROPERTY(Transient)
	TObjectPtr<class UBD_AbilitySystemComponent> AbilitySystemComponent;

	//UPROPERTY(Category = "GAS")
	//TObjectPtr<class UBD_HealthAttributeSet> HealthSet;

	//UPROPERTY(Category = "GAS")
	//TObjectPtr<class UBD_CombatAttributeSet> CombatSet;

	FSimpleMulticastDelegate OnAbilitySystemInitialized;
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
};
