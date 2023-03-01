// Fill out your copyright notice in the Description page of Project Settings.
#include "BombProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Interface/MonsterInterface.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

ABombProjectile::ABombProjectile()
{
	if (PMC)
	{
		PMC->InitialSpeed = 1000.f;
		PMC->MaxSpeed = 1000.f;
		PMC->ProjectileGravityScale = 1.f;
		PMC->bShouldBounce = true;
	}
}

void ABombProjectile::SetVelocity(const FVector& Dir)
{
	if (PMC)
	{
		PMC->Velocity = Dir;
	}
}

void ABombProjectile::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor == GetOwner()) return;
		if (OtherActor->Implements<UMonsterInterface>() && GetOwner()->Implements<UMonsterInterface>()) return;

		if (Cast<ABaseCharacter>(OtherActor)) //맞은게 상대
		{
			Cast<ABaseCharacter>(OtherActor)->OnHit(CombatManager, ItemManager, GetOwner(), this, UCurrentHPRatioDamageType::StaticClass());
			CheckAttackerBeHealed(OtherActor, Cast<APlayerCharacter>(GetOwner()));
			PlayHitEffect();
		}
		else if (OtherActor != GetOwner()) //나 아닌 다른 무언가
		{
			PlayDestroyEffect();
		}
	}
}
