// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterPrimeHelix.h"

void AMonsterPrimeHelix::SpecialAttack(AActor* Target)
{
	Meteor(GetMeteorActorClass(), Target);
}