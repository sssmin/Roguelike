// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskSpeicalAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Roguelike/Character/Monster/MonsterCharacter.h"
#include "AIController.h"

UBTTaskSpeicalAttack::UBTTaskSpeicalAttack()
{
	TargetKey = "Target";
	CanSpecialAttackKey = "CanSpecialAttack";
}

EBTNodeResult::Type UBTTaskSpeicalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AMonsterCharacter* AIPawn = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AIPawn && BBComp)
	{
		AActor* Target = Cast<AActor>(BBComp->GetValueAsObject(TargetKey));
		
		AIPawn->SpecialAttack(Target);
		BBComp->SetValueAsBool(CanSpecialAttackKey, false);
		return EBTNodeResult::Succeeded;
	}
	UE_LOG(LogTemp, Warning, TEXT("Attack Failed"));
	return EBTNodeResult::Failed;
}
