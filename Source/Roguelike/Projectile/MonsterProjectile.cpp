// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AMonsterProjectile::AMonsterProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	if (PMC)
	{
		PMC->InitialSpeed = 1200.f;
		PMC->MaxSpeed = 1200.f;
	}
}

void AMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnParticle();
}

void AMonsterProjectile::Destroyed()
{
	if (GetHitParticle())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetHitParticle(), GetActorTransform());
	}
}