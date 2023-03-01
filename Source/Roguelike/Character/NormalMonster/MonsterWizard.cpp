// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterWizard.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

void AMonsterWizard::SpecialAttack(AActor* Target)
{
	Fire3Projectile(USpecialATKDamageType::StaticClass());
}
