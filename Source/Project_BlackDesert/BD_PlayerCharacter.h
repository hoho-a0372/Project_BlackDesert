// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BD_BaseCharacter.h"
#include "GameplayTagContainer.h"
#include "BD_PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECT_BLACKDESERT_API ABD_PlayerCharacter : public ABD_BaseCharacter
{ 
	GENERATED_BODY()
	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (ClampMin = "0", ClampMax = "1000", Units = "cm"))
	float DefaultCameraDistance = 400.f;

	// input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UInputAction> DodgeAction;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (Categories = "InputTag"))
	FGameplayTag JumpInputTag; // InputTag.Action.Jump 

	// UI
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowprivateAccess = "true"))
	//TSubclassOf<class UUserWidget> HealthBarWidgetClass;
	//UPROPERTY()
	//class UUserWidget* HealthBarWidgetInstance;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UWidgetComponent> HealthBar;
	//UPROPERTY(EditAnywhere, Category = "UI")
	//TObjectPtr<class UBD_HealthBar> HealthBarWidget;

public:
	ABD_PlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const struct FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpInputPressed();
	void JumpInputReleased();
	void SprintStart();
	void SprintEnd();
	//void Dodge(const FInputActionValue& Value);
	//void Interact(const FInputActionValue& Value);
};
