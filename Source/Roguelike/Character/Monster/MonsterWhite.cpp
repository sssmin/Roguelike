// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterWhite.h"

void AMonsterWhite::SpecialAttack(AActor* Target)
{
	
}

void AMonsterWhite::Destroyed()
{
	Super::Destroyed();

	FireIn8Parts();
}