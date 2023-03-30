// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterTurret.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"
#include "Sound/SoundCue.h"

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
	RLAIController = Cast<ARLMonsterAIController>(GetController());
	if (RLAIController && TurretBT)
	{
		RLAIController->SetBehaviorTree(TurretBT);
	}
}

void AMonsterTurret::SpecialAttack(AActor* Target)
{
	FireSpread8PartsFromCenter(USpecialATKDamageType::StaticClass());
}

void AMonsterTurret::Deactivate()
{
	Super::Deactivate();

	if (DestroySoundCue && DestroyParticle)
	{
		UGameplayStatics::PlaySound2D(this, DestroySoundCue);
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyParticle, GetActorLocation());
	}
	
}
