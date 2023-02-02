// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Roguelike/Component/CombatComponent.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Projectile/BaseProjectile.h"


ABaseCharacter::ABaseCharacter()
{
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
	ManagerComp = CreateDefaultSubobject<UManagerComponent>(TEXT("ManagerComp"));

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CombatComp && ProjectileClass)
	{
		CombatComp->SetProjectileClass(ProjectileClass);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	
	
	
	
}

void ABaseCharacter::Attack()
{
	if (CombatComp)
	{
		CombatComp->Fire();
	}
}

