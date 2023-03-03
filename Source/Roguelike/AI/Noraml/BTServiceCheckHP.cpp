// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckHP.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Component/MonsterCombatComponent.h"

void UBTServiceCheckHP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	
	if (BBComp && BBComp->GetValueAsBool("DestroyTurret")) return;
	
	if (ManagerComp)
	{
		if (ManagerComp->IsHPLow())
		{
			UMonsterCombatComponent* MonsterCombatComp = UMonsterCombatComponent::GetMonsterCombatComp(OwnerComp.GetAIOwner()->GetPawn());
			if (MonsterCombatComp)
			{
				MonsterCombatComp->RequestHeal();
			}
		}
	}
}
