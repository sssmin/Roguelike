// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerProjectile.h"
#include "Kismet/GameplayStatics.h"

APlayerProjectile::APlayerProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (!ProjectileParticles.IsEmpty())
	{
		switch (CombatManager.Element)
		{
		case EElement::NONE:
			SetProjectileParticle(ProjectileParticles[0]);
			SetHitParticle(HitParticles[0]);
			break;
		case EElement::FIRE:
			SetProjectileParticle(ProjectileParticles[1]);
			SetHitParticle(HitParticles[1]);
			break;
		case EElement::WATER:
			SetProjectileParticle(ProjectileParticles[2]);
			SetHitParticle(HitParticles[2]);
			break;
		case EElement::EARTH:
			SetProjectileParticle(ProjectileParticles[3]);
			SetHitParticle(HitParticles[3]);
			break;
		case EElement::DARKNESS:
			SetProjectileParticle(ProjectileParticles[4]);
			SetHitParticle(HitParticles[4]);
			break;
		case EElement::LIGHT:
			SetProjectileParticle(ProjectileParticles[5]);
			SetHitParticle(HitParticles[5]);
			break;
		}
	}

	SpawnParticle();
}


void APlayerProjectile::Destroyed()
{
	if (GetHitParticle())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetHitParticle(), GetActorTransform());
	}
}