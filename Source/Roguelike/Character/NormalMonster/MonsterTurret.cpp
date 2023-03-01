// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterTurret.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Projectile/TurretProjectile.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

AMonsterTurret::AMonsterTurret()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_TurretMonster"));
	if (BTObject.Succeeded())
	{
		TurretBT = BTObject.Object;
	}
}

void AMonsterTurret::GiveBTToController()
{
	RLAIController = RLAIController == nullptr ? Cast<ARLMonsterAIController>(GetController()) : RLAIController;
	if (RLAIController && RLAIController->GetBlackboardComponent())
	{
		RLAIController->SetBehaviorTree(TurretBT);
	}
}

void AMonsterTurret::SpecialAttack(AActor* Target)
{
	FireSpread8PartsFromCenter<ATurretProjectile>(USpecialATKDamageType::StaticClass(), GetTurretProjectileClass());
}

void AMonsterTurret::Destroyed()
{
	if (RLAIController && RLAIController->GetBlackboardComponent())
	{
		RLAIController->GetBlackboardComponent()->SetValueAsBool("DestroyTurret", true);
	}
}

