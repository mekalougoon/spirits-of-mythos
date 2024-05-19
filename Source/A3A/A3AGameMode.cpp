// Copyright Epic Games, Inc. All Rights Reserved.

#include "A3AGameMode.h"
#include "A3ACharacter.h"
#include "Brazier.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AA3AGameMode::AA3AGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AA3AGameMode::TurnOnLights()
{
	TArray<AActor*> Lights;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABrazier::StaticClass(), Lights);
	for (int i = 0; i < Lights.Num(); i++)
	{
		Cast<ABrazier>(Lights[i])->TurnOn();
	}
}

void AA3AGameMode::TurnOffLights()
{
	TArray<AActor*> Lights;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABrazier::StaticClass(), Lights);
	for (int i = 0; i < Lights.Num(); i++)
	{
		Cast<ABrazier>(Lights[i])->TurnOff();
	}
}
