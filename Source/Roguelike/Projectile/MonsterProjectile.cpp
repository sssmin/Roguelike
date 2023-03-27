// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AMonsterProjectile::AMonsterProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	if (PMC)
	{
		PMC->InitialSpeed = 1200.f;
		PMC->MaxSpeed = 1200.f;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> Sound(TEXT("/Game/Resource/Sound/Cue/MonsterNormalAttack_Cue"));
	if (Sound.Succeeded())
	{
		SpawnSoundCue = Sound.Object;
	}
		
}

void AMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnParticle();
	if (SpawnSoundCue)
	{
		UGameplayStatics::PlaySound2D(this, SpawnSoundCue);
	}
}

void AMonsterProjectile::Destroyed()
{
	if (GetHitParticle())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetHitParticle(), GetActorTransform());
	}
}