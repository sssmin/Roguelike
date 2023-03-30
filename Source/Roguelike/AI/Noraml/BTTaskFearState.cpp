// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskFearState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTaskFearState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	
	
	if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
	{
		AActor* Target = Cast<AActor>(BBComp->GetValueAsObject("Target"));
		APawn* Monster = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
		BBComp->SetValueAsObject("Target", nullptr);
		if (Target && Monster)
		{
			FVector ReverseToTargetVector = Monster->GetActorLocation() - Target->GetActorLocation();
			ReverseToTargetVector = ReverseToTargetVector.GetSafeNormal();
			const FVector ToGoVector = Monster->GetActorLocation() + ReverseToTargetVector * 1000.f;
			BBComp->SetValueAsVector("FearLocation", ToGoVector);
		}
	}
	return EBTNodeResult::Succeeded;
}
