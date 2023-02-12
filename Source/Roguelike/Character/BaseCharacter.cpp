// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Roguelike/Component/CombatComponent.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Projectile/BaseProjectile.h"


ABaseCharacter::ABaseCharacter()
{
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
	ManagerComponent = CreateDefaultSubobject<UManagerComponent>(TEXT("ManagerComp"));

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterBlockProjectile"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CombatComponent && ProjectileClass)
	{
		CombatComponent->SetProjectileClass(ProjectileClass);
		CombatComponent->GetCombatManager.BindUObject(this, &ThisClass::GetCombatManager);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
}

void ABaseCharacter::Attack()
{
	if (CombatComponent)
	{
		CombatComponent->ReadyToFire(true);
	}
}

void ABaseCharacter::OnHit(AActor* Attacker, const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager)
{
	if (ManagerComponent)
	{
		ManagerComponent->ReceiveDamage(EnemyCombatManager, EnemyItemManager);
	}
}

FCombatManager ABaseCharacter::GetCombatManager() const
{
	if (ManagerComponent)
	{
		return ManagerComponent->GetCombatManager();
	}
	return FCombatManager();
}


void ABaseCharacter::Dead()
{
	GetMovementComponent()->SetActive(false);
}
