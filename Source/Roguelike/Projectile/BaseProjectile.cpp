// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Roguelike/Character/BaseCharacter.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_CharacterBlockProjectile, ECollisionResponse::ECR_Block);
	Sphere->SetCollisionResponseToChannel(ECC_WallBlockProjectile, ECollisionResponse::ECR_Block);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);

	PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMC"));
	if (PMC)
	{
		PMC->InitialSpeed = 5000.f;
		PMC->MaxSpeed = 5000.f;
		PMC->bRotationFollowsVelocity = true;
		PMC->bShouldBounce = false;
	}
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ProjectileParticles.IsEmpty())
	{
		UParticleSystem* Particle = nullptr;
		switch (CombatManage.Element)
		{
			case EElement::NONE:
				Particle = ProjectileParticles[0];
				break;
			case EElement::FIRE:
				Particle = ProjectileParticles[1];
				break;
			case EElement::WATER:
				Particle = ProjectileParticles[2];
				break;
			case EElement::EARTH:
				Particle = ProjectileParticles[3];
				break;
			case EElement::DARKNESS:
				Particle = ProjectileParticles[4];
				break;
			case EElement::LIGHT:
				Particle = ProjectileParticles[5];
				break;
		}
		if (Particle)
		{
			UGameplayStatics::SpawnEmitterAttached(
				Particle,
				RootComponent,
				NAME_None,
				GetActorLocation(),
				GetActorRotation(),
				EAttachLocation::KeepWorldPosition);
		}
	}

	if (Sphere)
	{
		Sphere->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	}
		
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::SetVelocity(const FVector& Dir)
{
	if (PMC)
	{
		PMC->Velocity = Dir * PMC->InitialSpeed;
	}
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && Cast<ABaseCharacter>(OtherActor) && (OtherActor != GetOwner()))
	{
		Cast<ABaseCharacter>(OtherActor)->OnHit(GetOwner(), CombatManage);
		PlayHitEffect();
	}
	else
	{
		PlayDestroyEffect();
	}
}

void ABaseProjectile::PlayHitEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("hit character"));
	Destroy();
}

void ABaseProjectile::PlayDestroyEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("hit other"));
	Destroy();
}

void ABaseProjectile::Destroyed()
{

}