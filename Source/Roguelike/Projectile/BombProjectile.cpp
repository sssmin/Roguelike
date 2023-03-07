// Fill out your copyright notice in the Description page of Project Settings.
#include "BombProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Interface/MonsterInterface.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

ABombProjectile::ABombProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	GetSphereComp()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetSphereComp()->SetCollisionObjectType(ECC_Projectile);
	GetSphereComp()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetSphereComp()->SetCollisionResponseToChannel(ECC_CharacterBlockProjectile, ECollisionResponse::ECR_Overlap);
	GetSphereComp()->SetCollisionResponseToChannel(ECC_WallBlockProjectile, ECollisionResponse::ECR_Block);
	GetSphereComp()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetSphereComp()->SetCollisionResponseToChannel(ECC_Floor, ECollisionResponse::ECR_Block);
	GetSphereComp()->SetSphereRadius(64.f);
	GetSphereComp()->SetGenerateOverlapEvents(true);
	
	if (PMC)
	{
		PMC->InitialSpeed = 1000.f;
		PMC->MaxSpeed = 1000.f;
		PMC->ProjectileGravityScale = 1.f;
		PMC->bShouldBounce = true;
	}
	
	BombTime = 4.f;
}

void ABombProjectile::SetVelocity(const FVector& Dir)
{
	if (PMC)
	{
		PMC->Velocity = Dir;
	}
}

void ABombProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle; 
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::BombSetTimer, BombTime, false);
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
	}
}

void ABombProjectile::BombSetTimer()
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	UGameplayStatics::ApplyRadialDamage(
				this,
				50,
				GetActorLocation(),
				250.f,
				USkillDamageType::StaticClass(),
				IgnoreActors,
				this,
				GetOwner()->GetInstigatorController(),
				true,
				ECollisionChannel::ECC_WorldStatic
				);
	PlayHitEffect();
}
