// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"

/*void UInGameWidget::NativeTick
(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Player)
	{
		//character health label
		HealthBar->SetPercent(Player->Health / 100);
		FNumberFormattingOptions Opts;
		Opts.SetMaximumFractionalDigits(0);
		CurrentHealthLabel->SetText(FText::AsNumber(Player->Health, &Opts));
	}
}*/

void UInGameWidget::UpdateName(FString UpdatedName)
{
	//character type label
	CurrentlyPossessedLabel->SetText(FText::FromString(UpdatedName));
}

void UInGameWidget::UpdateHealth()
{
	if (Player)
	{
		//character health label
		HealthBar->SetPercent(Player->Health / 100);
		FNumberFormattingOptions Opts;
		Opts.SetMaximumFractionalDigits(0);
		CurrentHealthLabel->SetText(FText::AsNumber(Player->Health, &Opts));
	}
}


void UInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<AA3ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
