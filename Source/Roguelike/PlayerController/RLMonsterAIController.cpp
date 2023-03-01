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

	TargetKey = "Target";
	OutofSightKey = "OutOfSight";

}

void ARLMonsterAIController::BeginPlay()
{
	Super::BeginPlay();


}

void ARLMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		if (Monster && (Monster->GetMonsterType() != EMonsterType::BOSS) && HPBarWidgetClass)
		{
			Monster->SetHPBarWidget(HPBarWidgetClass);
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
			BBComp->SetValueAsObject(TargetKey, Actor);
			BBComp->SetValueAsBool(OutofSightKey, false);
		}
		else //시야 놓침.
		{
			BBComp->SetValueAsBool(OutofSightKey, true);
		}
	}
}

void ARLMonsterAIController::SetBehaviorTree(UBehaviorTree* BT)
{
	BehaviorTree = BT;

	if (BehaviorTree && BBComp && BTComp)
	{
		BBComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::RunBT, FMath::RandRange(0.5f, 1.f), false);
	}
}

void ARLMonsterAIController::RunBT()
{
	RunBehaviorTree(BehaviorTree);
}

void ARLMonsterAIController::SetTarget(AActor* Target)
{
	if (Target && BBComp)
	{
		BBComp->SetValueAsObject(TargetKey, Target);
	}
}

void ARLMonsterAIController::OnSkillEnd()
{
	if (BBComp)
	{
		BBComp->SetValueAsBool("MustStop", false);
	}
}