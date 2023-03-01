// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskFearState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTaskFearState::UBTTaskFearState()
{
	LocKey = "FearLocation";
}

EBTNodeResult::Type UBTTaskFearState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* Monster = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp)
	{
		AActor* Target = Cast<AActor>(BBComp->GetValueAsObject("Target"));
		BBComp->SetValueAsObject("Target", nullptr);
		if (Target && Monster)
		{
			FVector ReverseToTargetVector = Monster->GetActorLocation() - Target->GetActorLocation();
			ReverseToTargetVector = ReverseToTargetVector.GetSafeNormal();
			FVector ToGoVector = Monster->GetActorLocation() + ReverseToTargetVector * 1000.f;
			BBComp->SetValueAsVector(LocKey, ToGoVector);
		}
	}
	

	
	return EBTNodeResult::Succeeded;
}
