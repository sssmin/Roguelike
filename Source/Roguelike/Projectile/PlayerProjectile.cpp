// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerProjectile.h"
#include "Kismet/GameplayStatics.h"

APlayerProjectile::APlayerProjectile()
{
}

void APlayerProjectile::SetParticle()
{
	if (!ProjectileParticles.IsEmpty())
	{
		switch (CombatManager.Element)
		{
		case EElement::NONE:
			ProjectileParticle = ProjectileParticles[0];
			HitParticle = HitParticles[0];
			break;
		case EElement::FIRE:
			ProjectileParticle = ProjectileParticles[1];
			HitParticle = HitParticles[1];
			break;
		case EElement::WATER:
			ProjectileParticle = ProjectileParticles[2];
			HitParticle = HitParticles[2];
			break;
		case EElement::EARTH:
			ProjectileParticle = ProjectileParticles[3];
			HitParticle = HitParticles[3];
			break;
		case EElement::DARKNESS:
			ProjectileParticle = ProjectileParticles[4];
			HitParticle = HitParticles[4];
			break;
		case EElement::LIGHT:
			ProjectileParticle = ProjectileParticles[5];
			HitParticle = HitParticles[5];
			break;
		}
	}

	Super::SetParticle();
}


void APlayerProjectile::Destroyed()
{
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorTransform());
	}

}