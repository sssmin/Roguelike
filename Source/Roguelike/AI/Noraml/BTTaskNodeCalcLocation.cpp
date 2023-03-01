// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskNodeCalcLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTaskNodeCalcLocation::UBTTaskNodeCalcLocation()
{
	LocationKey = "RangedLocation";
	TargetKey = "Target";
}

EBTNodeResult::Type UBTTaskNodeCalcLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AIPawn && BBComp)
	{
		APawn* Target = Cast<APawn>(BBComp->GetValueAsObject(TargetKey));

		//패트롤은 랜덤 방향, 짧게 짧게
		//여기는 이동할 방향 정하고, 얼마나 이동할지. 
		if (Target)
		{
			const float Dist = AIPawn->GetDistanceTo(Target);
		}


	}
	
	return EBTNodeResult::Failed;
}
