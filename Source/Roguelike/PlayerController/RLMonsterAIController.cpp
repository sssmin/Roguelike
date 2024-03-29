// Fill out your copyright notice in the Description page of Project Settings.
#include "RLMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Widget/HPBarWidget.h"
#include "Roguelike/Widget/BossHPBarWidget.h"


ARLMonsterAIController::ARLMonsterAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	BBComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComp"));
	BTComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp")));

	AISightRadius = 2000.f;
	AISightAge = 2.f;
	AILoseSightRadius = AISightRadius + 50.f;
	AIFieldOfView = 90.f;

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	
}

void ARLMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ARLMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetPerceptionComponent())
	{
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnPercptionUpdated);
	}
	
	if (InPawn)
	{
		Monster = Cast<AMonsterCharacter>(InPawn);
		if (Monster && HPBarWidgetClass && BossHPBarWidgetClass)
		{
			if (Monster->GetMonsterType() == EMonsterType::Ego) return;
			
			if (Monster->GetMonsterType() == EMonsterType::Boss)
			{
				Monster->SetHPBarWidget(BossHPBarWidgetClass);
			}
			else
			{
				Monster->SetHPBarWidget(HPBarWidgetClass);
			}
		}
	}
}

void ARLMonsterAIController::OnPercptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	check(BBComp);
	if (Cast<APlayerCharacter>(Actor) && !Cast<APlayerCharacter>(Actor)->IsDead())
	{
		if (Stimulus.IsActive())
		{
			BBComp->SetValueAsObject("Target", Actor);
			BBComp->SetValueAsBool("OutOfSight", false);
		}
		else //시야 놓침.
		{
			BBComp->SetValueAsBool("OutOfSight", true);
		}
	}
}

void ARLMonsterAIController::SetBehaviorTree(UBehaviorTree* BT)
{
	BehaviorTree = BT;
	
	if (BehaviorTree && BBComp && BTComp)
	{
		bool Result = BBComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::RunBT, FMath::RandRange(0.5f, 1.f), false);
	}
}

void ARLMonsterAIController::RunBT()
{
	RunBehaviorTree(BehaviorTree);
}

AActor* ARLMonsterAIController::GetTarget() const
{
	if (BBComp)
	{
		return Cast<AActor>(BBComp->GetValueAsObject("Target"));
	}
	return nullptr;
}

void ARLMonsterAIController::SetTarget(AActor* Target)
{
	if (Target && BBComp)
	{
		BBComp->SetValueAsObject("Target", Target);
	}
}

void ARLMonsterAIController::OnSkillEnd()
{
	if (BBComp)
	{
		BBComp->SetValueAsBool("MustStop", false);
	}
}