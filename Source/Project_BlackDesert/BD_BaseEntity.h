// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BD_BaseEntity.generated.h"

UCLASS(config=Game)
class PROJECT_BLACKDESERT_API ABD_BaseEntity : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ABD_BaseEntity();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<class UBD_AbilitySystemComponent> AbilitySystemComp;

	UPROPERTY()
	TObjectPtr<class UBD_HealthAttributeSet> HealthSet;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
