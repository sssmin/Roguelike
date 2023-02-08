// Fill out your copyright notice in the Description page of Project Settings.


#include "HealItem.h"
#include "Roguelike/Character/PlayerCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AHealItem::AHealItem()
{
	SetItemType(EItemType::STACK_ITEM);
}

void AHealItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemParticle && GetWorld())
	{
		HealItemParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ItemParticle, GetActorTransform());
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
			if (HealItemParticleComp)
			{
				HealItemParticleComp->DestroyComponent();
			}
			Destroy();
		}
	}
}