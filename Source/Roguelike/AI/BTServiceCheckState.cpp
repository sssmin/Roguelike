// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Roguelike/Component/ManagerComponent.h"

UBTServiceCheckState::UBTServiceCheckState()
{
	StateKey.SelectedKeyName = FName("State");
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
		APawn* AIPawn = AIC->GetPawn();
		
		if (AIPawn)
		{
			UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(AIPawn);
			if (ManagerComp)
			{
				CurrentState = ManagerComp->GetCurrentState();

				if (CurrentState & static_cast<uint8>(EState::FROZEN))
				{
					BBComp->SetValueAsEnum(StateKey.SelectedKeyName, MonsterState::FROZEN);
				}
				if (CurrentState & static_cast<uint8>(EState::FEAR))
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
