// Fill out your copyright notice in the Description page of Project Settings.


#include "KillPlayerTask.h"

#include "GuardAIController.h"

EBTNodeResult::Type UKillPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;
	if (!Component)
		return EBTNodeResult::Failed;

	AGuardAIController* MyController = Cast<AGuardAIController>(Component->GetOwner());
	if (!MyController)
		return EBTNodeResult::Failed;


	MyController->RemoveHealth();
	return EBTNodeResult::Succeeded;
}
