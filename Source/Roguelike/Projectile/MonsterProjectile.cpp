// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AMonsterProjectile::AMonsterProjectile()
{
	if (PMC)
	{
		PMC->InitialSpeed = 1200.f;
		PMC->MaxSpeed = 1200.f;
	}
}

void AMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();


	SetParticle(MonsterProjectileParticle);

	SpawnParticle();
}


void AMonsterProjectile::Destroyed()
{
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorTransform());
	}
}