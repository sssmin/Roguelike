// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTaskMoveTo::PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardKey.SelectedKeyName;

	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp)
	{

		BBComp->SetValueAsBool("CanUseSkill", false);


		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
	
	return EBTNodeResult::Type();
}
