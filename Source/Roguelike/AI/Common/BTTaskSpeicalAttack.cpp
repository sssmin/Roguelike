// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskSpeicalAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"

EBTNodeResult::Type UBTTaskSpeicalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AMonsterCharacter* AIPawn = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AIPawn && BBComp)
	{
		AActor* Target = Cast<AActor>(BBComp->GetValueAsObject("Target"));
		
		AIPawn->SpecialAttack(Target);
		BBComp->SetValueAsBool("CanSpecialAttack", false);
		return EBTNodeResult::Succeeded;
	}
	UE_LOG(LogTemp, Warning, TEXT("Attack Failed"));
	return EBTNodeResult::Failed;
}
