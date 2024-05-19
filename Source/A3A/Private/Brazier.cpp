// Fill out your copyright notice in the Description page of Project Settings.


#include "Brazier.h"

#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"

// Sets default values
ABrazier::ABrazier()
{
	BottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomMesh"));
	RootComponent = BottomMesh;
	MiddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MiddleMesh"));
	MiddleMesh->SetupAttachment(BottomMesh);
	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(BottomMesh);
	FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireMesh"));
	FireMesh->SetupAttachment(BottomMesh);
	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->SetIntensity(0.f);
	PointLightComponent->SetLightColor(FLinearColor(0.0f, 0.0f, 0.0f));
	PointLightComponent->SetupAttachment(FireMesh);
	PointLightComponent->SetRelativeLocation(FVector(0,0,-20));
}

// Called when the game starts or when spawned
void ABrazier::BeginPlay()
{
	Super::BeginPlay();
	if (Material)
	{
		MatInstance = UMaterialInstanceDynamic::Create(Material, this);
		FireMesh->SetMaterial(0, MatInstance);
	}
}

void ABrazier::TurnOn()
{
	MatInstance->SetScalarParameterValue("Emissive", 1);
	MatInstance->SetScalarParameterValue("Opacity", 1);
	PointLightComponent->SetIntensity(5000.f);
}

void ABrazier::TurnOff()
{
	MatInstance->SetScalarParameterValue("Emissive", 0);
	MatInstance->SetScalarParameterValue("Opacity", 0);
	PointLightComponent->SetIntensity(0.f);
}
