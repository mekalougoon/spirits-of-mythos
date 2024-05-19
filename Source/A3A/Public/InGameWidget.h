// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "A3ACharacter.h"
#include "InGameWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class A3A_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

	AA3ACharacter* Player;
	virtual void NativeConstruct() override;
	/*virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;*/

public:
	void UpdateName(FString UpdatedName);

	void UpdateHealth();

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHealthLabel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentlyPossessedLabel;
};
