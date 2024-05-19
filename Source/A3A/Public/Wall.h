// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "A3ACharacter.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

UCLASS()
class A3A_API AWall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category = "Material")
	UStaticMeshComponent* WallMesh;

	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category = "Material")
	UStaticMeshComponent* TopMesh;

	UPROPERTY
	(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* Material;

	AA3ACharacter* Player;

	UMaterialInstanceDynamic* MatInstance;

	UPROPERTY(EditAnywhere)
	int TimesHit = 0;

	UPROPERTY(EditAnywhere)
	float HealthAlteration = 0.0f;

public:
	UFUNCTION()
	void ChangeHealth();
};
