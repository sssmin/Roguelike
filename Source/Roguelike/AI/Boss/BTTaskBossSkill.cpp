// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskBossSkill.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Roguelike/Character/BossMonster/BossMonsterCharacter.h"
#include "AIController.h"

EBTNodeResult::Type UBTTaskBossSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossMonsterCharacter* AIPawn = Cast<ABossMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AIPawn && BBComp)
	{
		AIPawn->ExecuteSkill();

		BBComp->SetValueAsBool("CanUseSkill", false);
		BBComp->SetValueAsBool("MustStop", true);


		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
