// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskNodeFire.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "AIController.h"


EBTNodeResult::Type UBTTaskNodeFire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterCharacter* AIPawn = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AIPawn && BBComp)
	{
		AIPawn->Attack();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
