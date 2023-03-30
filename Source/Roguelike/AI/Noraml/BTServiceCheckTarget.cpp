// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Roguelike/Character/BaseCharacter.h"

void UBTServiceCheckTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
	{
		if (ABaseCharacter* Target = Cast<ABaseCharacter>(BBComp->GetValueAsObject("Target")))
		{
			if (Target->IsDead())
			{
				BBComp->SetValueAsObject("Target", nullptr);
			}
		}
	}
}
