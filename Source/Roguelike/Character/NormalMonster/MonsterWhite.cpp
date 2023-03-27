// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterWhite.h"

#include "Kismet/GameplayStatics.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"
#include "Sound/SoundCue.h"


void AMonsterWhite::SpecialAttack(AActor* Target)
{
}

void AMonsterWhite::Dead()
{
	Super::Dead();
	
	FireIn8Parts(USpecialATKDamageType::StaticClass());
	if (DestroySoundCue)
	{
		UGameplayStatics::PlaySound2D(this, DestroySoundCue);
	}
}


