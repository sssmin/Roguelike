// Fill out your copyright notice in the Description page of Project Settings.
#include "ThrowBallProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Interface/MonsterInterface.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

AThrowBallProjectile::AThrowBallProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	if (PMC)
	{
		PMC->InitialSpeed = 3000.f;
		PMC->MaxSpeed = 3000.f;
		PMC->ProjectileGravityScale = 0.f;
		PMC->bRotationFollowsVelocity = true;
		PMC->bShouldBounce = false;
	}
}

void AThrowBallProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AThrowBallProjectile::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor == GetOwner()) return;
		if (OtherActor->Implements<UMonsterInterface>() && GetOwner()->Implements<UMonsterInterface>()) return;

		if (Cast<ABaseCharacter>(OtherActor)) //맞은게 상대
		{
			Cast<ABaseCharacter>(OtherActor)->OnHit(CombatManager, ItemManager, GetOwner(), this, UMaxHPRatioDamageType::StaticClass());
			CheckAttackerBeHealed(OtherActor, Cast<APlayerCharacter>(GetOwner()));
			PlayHitEffect();
		}
		else if (OtherActor != GetOwner()) //나 아닌 다른 무언가
		{
			PlayDestroyEffect();
		}
	}
}
