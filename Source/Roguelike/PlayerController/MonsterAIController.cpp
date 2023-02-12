// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AMonsterAIController::AMonsterAIController()
{
	BBComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComp"));
	BTComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComp"));
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

   /* if (InPawn)
    {
        EnemyCharacter = Cast<AEnemyCharacter>(InPawn);
        if (EnemyCharacter)
        {
            if (EnemyCharacter->GetBT() && BBComp)
            {
                BBComp->InitializeBlackboard(*(EnemyCharacter->GetBT()->BlackboardAsset));
                RunBehaviorTree(EnemyCharacter->GetBT());
                BBComp->SetValueAsBool("CanAttack", true);
                BBComp->SetValueAsInt("PatrolIndex", 0);
                BBComp->SetValueAsBool("SpecialReady", true);
                BBComp->SetValueAsBool("SpecialComplete", true);
            }
        }
    }*/
}