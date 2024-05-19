// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Dynamite.generated.h"

UENUM(BlueprintType)
enum class EGravityType:uint8
{
	GE_Push UMETA(DisplayName="Explode"),
	GE_Down UMETA(DisplayName="Pull Down"),
};


UCLASS()
class A3A_API ADynamite : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADynamite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY
	(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	EGravityType GravityType;

	UPROPERTY
	(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	float SweepSize = 1000.0f;

	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	UStaticMeshComponent* EndMesh;

	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	UStaticMeshComponent* StringMesh;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void Interact();

	virtual void Interact_Implementation() override;
};
