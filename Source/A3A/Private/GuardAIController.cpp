// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardAIController.h"

#include "Ghoul.h"

AGuardAIController::AGuardAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Eyes")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);
	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AGuardAIController::OnSensesUpdated);
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ChaseNoise Component"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetAutoActivate(false);
}

void AGuardAIController::BeginPlay()
{
	Super::BeginPlay();
	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, ReturnLocation);
		MoveToLocation(ReturnLocation.Location);
	}

	if (!AIBlackboard)
		return;

	if (!ensure(BehaviorTree))
		return;

	UseBlackboard(AIBlackboard, BlackboardComponent);

	RunBehaviorTree(BehaviorTree);
}

void AGuardAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AGuardAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
}

FRotator AGuardAIController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}
	return FRotator::ZeroRotator;
}

void AGuardAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	//Super::OnMoveCompleted(RequestID, Result);
	/*if(NavigationSystem)
	 {
		 FNavLocation ReturnLocation;
		 NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(),
		2000,ReturnLocation);
		 MoveToLocation(ReturnLocation.Location);*/
}

bool AGuardAIController::IsPlayerInvisible(AActor* OtherActor)
{
	APawn* TemporaryPawn = Cast<APawn>(OtherActor);
	AGhoul* PlayerCharacter = Cast<AGhoul>(OtherActor);

	if (TemporaryPawn && TemporaryPawn->IsPlayerControlled() && TemporaryPawn->IsA(AGhoul::StaticClass()) == false)
	{
		return true;
	}

	else if (PlayerCharacter->GetAbilityStatus() == false)
	{
		return true;
	}

	else
	{
		return false;
	}
}


void AGuardAIController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	APawn* TemporaryPawn = Cast<APawn>(UpdatedActor);

	if (IsPlayerInvisible(TemporaryPawn) == true)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Actor Location"));
			TargetPlayer = TemporaryPawn;
			BlackboardComponent->SetValueAsBool("ChasePlayer", true);
			BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
			//bIsChaseNoisePlaying = true;
			StartChaseNoise();
		}
		else
		{
			TargetPlayer = nullptr;
			BlackboardComponent->ClearValue("ChasePlayer");
			//bIsChaseNoisePlaying = false;
			ResetChaseNoise();
		}
	}
	else
	{
		TargetPlayer = nullptr;
		BlackboardComponent->ClearValue("ChasePlayer");
	}
}

void AGuardAIController::GenerateNewRandomLocation()
{
	if (NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint", ReturnLocation.Location);
	}
}

void AGuardAIController::RemoveHealth()
{
	Cast<AA3ACharacter>(TargetPlayer)->ChangeHealth(-0.5);
	UE_LOG(LogTemp, Warning, TEXT("AI KILLING"));
}

void AGuardAIController::StartChaseNoise()
{
	float DistanceToPlayer = FVector::Distance(Cast<AA3ACharacter>(TargetPlayer)->GetActorLocation(), GetPawn()->GetActorLocation());
	AudioComponent->Play();
	AudioComponent->SetFloatParameter("RepeatSpeed", (1-DistanceToPlayer/1000));
	bIsChaseNoisePlaying = true;
}

void AGuardAIController::ResetChaseNoise()
{
	if(AudioComponent->Sound)
	{
		AudioComponent->SetTriggerParameter(FName("On Stop"));
		bIsChaseNoisePlaying = false;
		AudioComponent->ResetParameters();
	}
}
