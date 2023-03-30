// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskMoveComplete.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTaskMoveComplete::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
	{
		BBComp->SetValueAsBool("ShouldMove", false);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
