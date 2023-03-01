// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterDino.h"
#include "Roguelike/Projectile/MonsterProjectile.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

void AMonsterDino::SpecialAttack(AActor* Target)
{
	FireIn3Parts<AMonsterProjectile>(USpecialATKDamageType::StaticClass(), nullptr);
}
