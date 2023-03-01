// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerProjectile.h"
#include "Kismet/GameplayStatics.h"

APlayerProjectile::APlayerProjectile()
{
}

void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (!ProjectileParticles.IsEmpty())
	{
		switch (CombatManager.Element)
		{
		case EElement::NONE:
			SetParticle(ProjectileParticles[0]);
			HitParticle = HitParticles[0];
			break;
		case EElement::FIRE:
			SetParticle(ProjectileParticles[1]);
			HitParticle = HitParticles[1];
			break;
		case EElement::WATER:
			SetParticle(ProjectileParticles[2]);
			HitParticle = HitParticles[2];
			break;
		case EElement::EARTH:
			SetParticle(ProjectileParticles[3]);
			HitParticle = HitParticles[3];
			break;
		case EElement::DARKNESS:
			SetParticle(ProjectileParticles[4]);
			HitParticle = HitParticles[4];
			break;
		case EElement::LIGHT:
			SetParticle(ProjectileParticles[5]);
			HitParticle = HitParticles[5];
			break;
		}
	}

	SpawnParticle();
}


void APlayerProjectile::Destroyed()
{
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorTransform());
	}

}