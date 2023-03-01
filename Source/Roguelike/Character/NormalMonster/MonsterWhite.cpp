// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterWhite.h"
#include "Roguelike/Projectile/MonsterProjectile.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"


void AMonsterWhite::SpecialAttack(AActor* Target)
{
	
}

void AMonsterWhite::Destroyed()
{
	Super::Destroyed();

	FireIn8Parts<AMonsterProjectile>(USpecialATKDamageType::StaticClass(),  nullptr);
}
