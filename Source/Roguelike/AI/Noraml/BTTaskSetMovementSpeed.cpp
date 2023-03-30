// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskSetMovementSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"

EBTNodeResult::Type UBTTaskSetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (AMonsterCharacter* AIPawn = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		switch (MovementType)
		{
		case EMovementType::DEFAULT:
			AIPawn->SetDefaultSpeed();
			break;
		case EMovementType::PATROL:
			AIPawn->SetPatrolSpeed();
			break;
		}
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
