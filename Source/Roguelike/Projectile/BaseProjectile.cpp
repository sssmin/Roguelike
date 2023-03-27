// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Game/RLMainGameMode.h"
#include "Roguelike/Interface/MonsterInterface.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_CharacterBlockProjectile, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WallBlockProjectile, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Floor, ECollisionResponse::ECR_Block);
	Sphere->SetSphereRadius(64.f);
	Sphere->SetGenerateOverlapEvents(true);

	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMC"));
	if (PMC)
	{
		PMC->InitialSpeed = 3000.f;
		PMC->MaxSpeed = 3000.f;
		PMC->ProjectileGravityScale = 0.f;
		PMC->bRotationFollowsVelocity = true;
		PMC->bShouldBounce = false;
	}

	Range = 0;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	if (Sphere)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnHit);
	}
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Dist = FVector::Distance(StartLocation, GetActorLocation());
	if (Range <= Dist)
	{
		Destroy();
	}
}

void ABaseProjectile::SetVelocity(const FVector& Dir)
{
	if (PMC)
	{
		PMC->Velocity = Dir * PMC->InitialSpeed;
	}
}

void ABaseProjectile::SetRange(float InRange)
{
	Range = InRange;
}

void ABaseProjectile::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor == GetOwner()) return;
		if (OtherActor->Implements<UMonsterInterface>() && GetOwner()->Implements<UMonsterInterface>()) return;
	
		if (Cast<ABaseCharacter>(OtherActor)) //맞은게 상대
		{
			Cast<ABaseCharacter>(OtherActor)->OnHit(CombatManager, ItemManager, GetOwner(), this, GetDamageType());
			CheckAttackerBeHealed(Cast<ABaseCharacter>(GetOwner()));
			PlayHitEffect();
		}
		else if (OtherActor != GetOwner()) //나 아닌 다른 무언가
		{
			PlayDestroyEffect();
		}
	}
}

void ABaseProjectile::PlayHitEffect()
{
	Destroy();
}

void ABaseProjectile::PlayDestroyEffect()
{
	Destroy();
}

void ABaseProjectile::Destroyed()
{
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorTransform());
	}
}

void ABaseProjectile::CheckAttackerBeHealed(ABaseCharacter* Attacker)
{
	if ((CombatManager.Element == EElement::Light))
	{
		if (Cast<AMonsterCharacter>(Attacker))
		{
			const float MonsterLightBuffHealPer = 15.f;
			if (FMath::RandRange(1.f, 100.f) <= MonsterLightBuffHealPer)
			{
				Attacker->HealByHit(5.f);
			}
		}
		else if (Cast<APlayerCharacter>(Attacker))
		{
			const float PlayerLightBuffHealPer = 8.75f;
			if (FMath::RandRange(1.f, 100.f) <= PlayerLightBuffHealPer)
			{
				Attacker->HealByHit(7.5f);
			}
		}
	}
}

void ABaseProjectile::SetCombatManage(const FCombatManager& InManager)
{
	CombatManager = InManager;
}


void ABaseProjectile::SpawnParticle()
{
	if (ProjectileParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(
			ProjectileParticle,
			RootComponent,
			NAME_None,
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition);
	}
}