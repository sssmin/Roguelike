// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckDistFromBoss.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTServiceCheckDistFromBoss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp)
	{
		AActor* Boss = Cast<AActor>(BBComp->GetValueAsObject("Boss"));
		if (Boss)
		{
			AActor* AIOwner = OwnerComp.GetAIOwner()->GetPawn();
			if(AIOwner)
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
