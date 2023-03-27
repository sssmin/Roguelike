// Fill out your copyright notice in the Description page of Project Settings.
#include "HealItem.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AHealItem::AHealItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHealItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemParticle && GetWorld())
	{
		HealItemParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ItemParticle, GetActorTransform());
	}
}

void AHealItem::Destroyed()
{
	Super::Destroyed();

	if (HealItemParticleComp)
	{
		HealItemParticleComp->DestroyComponent();
	}
}

void AHealItem::ItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Cast<APlayerCharacter>(OtherActor))
	{
		Cast<APlayerCharacter>(OtherActor)->HealByItem();
		if (HealedParticle && GetWorld())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealedParticle, GetActorTransform());
			if (DestroySoundCue)
			{
				UGameplayStatics::PlaySound2D(this, DestroySoundCue);
			}
			if (HealItemParticleComp)
			{
				HealItemParticleComp->DestroyComponent();
			}
			Destroy();
		}
	}
}