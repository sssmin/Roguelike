// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskFearState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTaskFearState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* Monster = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if ((nullptr == Monster) || (nullptr == BBComp)) return EBTNodeResult::Failed;

	BBComp->SetValueAsObject("Target", nullptr);
	
	
	return EBTNodeResult::Succeeded;
}
