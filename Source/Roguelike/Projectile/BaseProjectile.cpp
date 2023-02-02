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
		//OtherActor에 대미지 적용
	}
}