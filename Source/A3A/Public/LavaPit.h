// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "LavaPit.generated.h"

UCLASS()
class A3A_API ALavaPit : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LavaPitMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* LavaPitHitBox;

	UPROPERTY(EditAnywhere)
	float HealthAlteration = 0.0f;

public:
	// Sets default values for this actor's properties
	ALavaPit();

	UFUNCTION()
	void OnHitBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	                          bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHitBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = Effects)
	UNiagaraSystem* NS_LavaPit;

	void SpawnNSSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
