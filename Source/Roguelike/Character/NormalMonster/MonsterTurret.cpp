// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterTurret.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Projectile/TurretProjectile.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

AMonsterTurret::AMonsterTurret()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_TurretMonster"));
	if (BTObject.Succeeded())
	{
		TurretBT = BTObject.Object;
	}
}

void AMonsterTurret::GiveBTToController()
{
	RLAIController = RLAIController == nullptr ? Cast<ARLMonsterAIController>(GetController()) : RLAIController;
	if (RLAIController)
	{
		RLAIController->SetBehaviorTree(TurretBT);
	}
}

void AMonsterTurret::SpecialAttack(AActor* Target)
{
	FireSpread8PartsFromCenter(USpecialATKDamageType::StaticClass());
}

