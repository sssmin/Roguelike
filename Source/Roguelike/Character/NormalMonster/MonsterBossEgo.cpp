// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterBossEgo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"
#include "Roguelike/Character/BossMonster/BossMonsterCharacter.h"

AMonsterBossEgo::AMonsterBossEgo()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_BossEgo"));
	if (BTObject.Succeeded())
	{
		EgoBT = BTObject.Object;
	}
}

void AMonsterBossEgo::GiveBTToController()
{
	RLAIController = Cast<ARLMonsterAIController>(GetController());
	
	if (RLAIController)
	{
		RLAIController->SetBehaviorTree(EgoBT);
	}
}

void AMonsterBossEgo::OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	if (ABossMonsterCharacter* BossCharacter = Cast<ABossMonsterCharacter>(Boss))
	{
		ARLMonsterAIController* BossController = Cast<ARLMonsterAIController>(BossCharacter->GetController());

		if (BossController && RLAIController)
		{
			BossController->SetTarget(Attacker);
			RLAIController->SetTarget(Attacker);
		}
	}
}

void AMonsterBossEgo::SetBoss(AActor* InBoss)
{
	Boss = InBoss;
	RLAIController = Cast<ARLMonsterAIController>(GetController());
	if (Boss && RLAIController && RLAIController->GetBlackboardComponent())
	{
		RLAIController->GetBlackboardComponent()->SetValueAsObject("Boss", Boss);
	}
}

void AMonsterBossEgo::SpecialAttack(AActor* Target)
{
	Super::SpecialAttack(Target);
}

