// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BD_HealthBar.generated.h"

/**
 * 
 */
UCLASS(abstract)
class PROJECT_BLACKDESERT_API UBD_HealthBar : public UUserWidget
{
	GENERATED_BODY()
	public:

	/** Sets the life bar to the provided 0-1 percentage value*/
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void SetHealthPercentage(float Percent);

	// Sets the life bar fill color
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void SetBarColor(FLinearColor Color);
};
