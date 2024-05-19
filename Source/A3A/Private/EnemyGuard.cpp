// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGuard.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyGuard::AEnemyGuard()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 5, 0);
}

// Called when the game starts or when spawned
void AEnemyGuard::BeginPlay()
{
	Super::BeginPlay();
}


// Called to bind functionality to input
void AEnemyGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
