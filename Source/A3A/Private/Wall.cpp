// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"

// Sets default values
AWall::AWall()
{
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh;
	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(WallMesh);
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AA3ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Material)
	{
		MatInstance = UMaterialInstanceDynamic::Create(Material, this);
		TopMesh->SetMaterial(0, MatInstance);
	}
}

void AWall::ChangeHealth()
{
	TimesHit++;
	if (Player && MatInstance)
	{
		switch (TimesHit)
		{
		case 1:
			MatInstance->SetScalarParameterValue("Emissive", 0.1);
			MatInstance->SetVectorParameterValue("Colour", FLinearColor(.25, 0, 0));
			UE_LOG(LogTemp, Warning, TEXT("Times hit: %d"), TimesHit);
			break;

		case 2:
			MatInstance->SetScalarParameterValue("Emissive", 0.2);
			MatInstance->SetVectorParameterValue("Colour", FLinearColor(.5, 0, 0));
			UE_LOG(LogTemp, Warning, TEXT("Times hit: %d"), TimesHit);
			break;

		case 3:
			MatInstance->SetScalarParameterValue("Emissive", 0.3);
			MatInstance->SetVectorParameterValue("Colour", FLinearColor(.75, 0, 0));
			UE_LOG(LogTemp, Warning, TEXT("Times hit: %d"), TimesHit);
			break;

		case 4:
			MatInstance->SetScalarParameterValue("Emissive", 4);
			MatInstance->SetVectorParameterValue("Colour", FLinearColor(1, 0, 0));
			UE_LOG(LogTemp, Warning, TEXT("Times hit: %d"), TimesHit);
			Destroy();
			break;

		default:
			break;
		}
	}
	/* on hit
reduce health
if health greater than 50, less than 100
show mat 1
if health less than 50, greater than 25
show mat 2
if health less than 25
show mat 3
if health 0
Destroy(); */
}
