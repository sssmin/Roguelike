// Fill out your copyright notice in the Description page of Project Settings.
#include "CombatComponent.h"

#include "Roguelike/Projectile/BaseProjectile.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCombatComponent::ReadyToFire(bool bPressed)
{
	
}


