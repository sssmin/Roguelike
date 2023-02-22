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
		PMC->MaxSpeed = 1700.f;
	}
}

void AMonsterProjectile::OneToTwo(const FVector& Dir)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ChangeDir"), Dir);
	float ChangeTime = 0.5f;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, ChangeTime, false);
}

void AMonsterProjectile::SetParticle()
{
	ProjectileParticle = MonsterProjectileParticle;
	
	Super::SetParticle();
}

void AMonsterProjectile::Destroyed()
{
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorTransform());
	}
}

void AMonsterProjectile::ChangeDir(const FVector& Dir)
{
	if (PMC)
	{
		FVector Dirs = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), Dir);
		PMC->SetVelocityInLocalSpace(Dir * PMC->MaxSpeed);
	}
}
