// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"

UBTServiceCheckState::UBTServiceCheckState()
{
	CurrentState = 0;
}

void UBTServiceCheckState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	CheckState(OwnerComp);
}

void UBTServiceCheckState::CheckState(UBehaviorTreeComponent& OwnerComp)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (AIC && BBComp)
	{
		if (APawn* AIPawn = AIC->GetPawn())
		{
			if (UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(AIPawn))
			{
				CurrentState = ManagerComp->GetCurrentState();
				
				if (CurrentState & static_cast<uint8>(EState::Frozen))
				{
					BBComp->SetValueAsEnum(StateKey.SelectedKeyName, MonsterState::FROZEN);
				}
				if (CurrentState & static_cast<uint8>(EState::Fear))
				{
					BBComp->SetValueAsEnum(StateKey.SelectedKeyName, MonsterState::FEAR);
				}
				if (CurrentState == 0)
				{
					BBComp->SetValueAsEnum(StateKey.SelectedKeyName, MonsterState::DEFAULT);
				}
			}
		}
	}
}

