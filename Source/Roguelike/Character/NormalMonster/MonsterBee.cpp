// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterBee.h"
#include "Components/CapsuleComponent.h"

#include "Roguelike/Projectile/OnetoAnotherProjectile.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"
#include "Roguelike/Roguelike.h"

AMonsterBee::AMonsterBee()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionObjectType(ECC_CharacterBlockProjectile);
}

void AMonsterBee::SpecialAttack(AActor* Target)
{
	FireOneToTwo(USpecialATKDamageType::StaticClass());
}
