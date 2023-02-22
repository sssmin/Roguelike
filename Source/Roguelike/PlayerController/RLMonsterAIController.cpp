// Fill out your copyright notice in the Description page of Project Settings.
#include "RLMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Character/Monster/MonsterCharacter.h"
#include "Roguelike/Widget/HPBarWidget.h"


ARLMonsterAIController::ARLMonsterAIController()
{
    PrimaryActorTick.bCanEverTick = true;

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

    bIsPlayerDetected = false;
    DistToPlayer = 0.f;

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
             if (BehaviorTree && BBComp && BTComp)
             {
                 BBComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
                 RunBehaviorTree(BehaviorTree);
             }
         }
     }
}




void ARLMonsterAIController::OnPercptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    check(BBComp);
    if (Cast<APlayerCharacter>(Actor))
    {
        if (Stimulus.IsActive())
        {
            UE_LOG(LogTemp, Warning, TEXT("Come In"));
            BBComp->SetValueAsObject(TargetKey, Actor);
            BBComp->SetValueAsBool(OutofSightKey, false);
        }
        else //시야 놓침.
        {
            UE_LOG(LogTemp, Warning, TEXT("Out"));
            BBComp->SetValueAsBool(OutofSightKey, true);
        }
    }
   
}
