// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckDistFromTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTServiceCheckDistFromTarget::UBTServiceCheckDistFromTarget()
{
	bNotifyBecomeRelevant = true;
}

void UBTServiceCheckDistFromTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
	{
		AActor* Target = Cast<AActor>(BBComp->GetValueAsObject("Target"));
		if (Target && OwnerComp.GetAIOwner())
		{
			if (AActor* AIOwner = OwnerComp.GetAIOwner()->GetPawn())
			{
				const float Dist = AIOwner->GetDistanceTo(Target);
				if (Dist >= 900.f)
				{
					const FVector Dir = (Target->GetActorLocation() - AIOwner->GetActorLocation()).GetSafeNormal();
					const FVector ToGoLoc = AIOwner->GetActorLocation() + Dir * (Dist - 900.f);
					BBComp->SetValueAsVector("ToGoLoc", ToGoLoc);
					BBComp->SetValueAsBool("ShouldMove", true);
				}
			}
		}
	}
}
