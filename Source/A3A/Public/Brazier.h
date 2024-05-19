// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "A3ACharacter.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"
#include "Brazier.generated.h"

UCLASS()
class A3A_API ABrazier : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABrazier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Material")
	UStaticMeshComponent* BottomMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Material")
	UStaticMeshComponent* MiddleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Material")
	UStaticMeshComponent* TopMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Material")
	UStaticMeshComponent* FireMesh;

	UPROPERTY(VisibleAnywhere)	
	UPointLightComponent* PointLightComponent;

	UPROPERTY
	(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* Material;

	AA3ACharacter* Player;

	UMaterialInstanceDynamic* MatInstance;


	UFUNCTION(BlueprintCallable)
	void TurnOn();

	UFUNCTION(BlueprintCallable)
	void TurnOff();
};
