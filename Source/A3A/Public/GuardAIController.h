// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ghoul.h"
#include "Components/AudioComponent.h"
#include "A3ACharacter.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GuardAIController.generated.h"

/**
 * 
 */
UCLASS()
class A3A_API AGuardAIController : public AAIController
{
	GENERATED_BODY()

	AGuardAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
	void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	UFUNCTION()
	bool IsPlayerInvisible(AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	bool bIsAIVisible;

public:
	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	float SightRadius = 500;

	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	float SightAge = 3.5;

	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	float LoseSightRadius = SightRadius + 30;

	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	float FieldOfView = 90;

	UPROPERTY
	(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	UAISenseConfig_Sight* SightConfiguration;

	UNavigationSystemV1* NavigationSystem;
	APawn* TargetPlayer;

	void GenerateNewRandomLocation();


	UPROPERTY
	(EditDefaultsOnly, Category = Blackboard)
	UBlackboardData* AIBlackboard;

	UPROPERTY
	(EditDefaultsOnly, Category = Blackboard)
	UBehaviorTree* BehaviorTree;

	UPROPERTY
	(EditDefaultsOnly, Category = Blackboard)
	UBlackboardComponent* BlackboardComponent;

	void RemoveHealth();

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent;
	
	bool bIsChaseNoisePlaying;

	void StartChaseNoise();

	void ResetChaseNoise();
};
