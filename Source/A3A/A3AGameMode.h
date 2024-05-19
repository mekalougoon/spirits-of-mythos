// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Brazier.h"
#include "GameFramework/GameModeBase.h"
#include "A3AGameMode.generated.h"

UCLASS(minimalapi)
class AA3AGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AA3AGameMode();

	UFUNCTION(BlueprintCallable)
	void TurnOnLights();

	UFUNCTION(BlueprintCallable)
	void TurnOffLights();
};
