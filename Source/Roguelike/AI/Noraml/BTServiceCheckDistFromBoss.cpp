// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckDistFromBoss.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTServiceCheckDistFromBoss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
	{
		if (AActor* Boss = Cast<AActor>(BBComp->GetValueAsObject("Boss")))
		{
			if(AActor* AIOwner = OwnerComp.GetAIOwner()->GetPawn())
			{
				const float Dist = Boss->GetDistanceTo(AIOwner);
				if (Dist >= 200.f)
				{
					const FVector Dir = (Boss->GetActorLocation() - AIOwner->GetActorLocation()).GetSafeNormal();
					const FVector ToGoLoc = AIOwner->GetActorLocation() + Dir * (Dist - 200.f);
					BBComp->SetValueAsVector("ToGoLoc", ToGoLoc);
					BBComp->SetValueAsBool("ShouldMove", true);
				}
			}
		}
	}
}
