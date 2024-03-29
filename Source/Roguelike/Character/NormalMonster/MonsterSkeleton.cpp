// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterSkeleton.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Projectile/BombProjectile.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

AMonsterSkeleton::AMonsterSkeleton()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_SkeletonMonster"));
	if (BTObject.Succeeded())
	{
		SkeletonBT = BTObject.Object;
	}
}

void AMonsterSkeleton::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterSkeleton::GiveBTToController()
{
	RLAIController = Cast<ARLMonsterAIController>(GetController());
	if (RLAIController && SkeletonBT)
	{
		RLAIController->SetBehaviorTree(SkeletonBT);
	}
}

void AMonsterSkeleton::SpecialAttack(AActor* Target)
{
	ThrowBomb(Target, USpecialATKDamageType::StaticClass());
}
