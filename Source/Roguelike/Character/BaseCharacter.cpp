// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Roguelike/Component/CombatComponent.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Projectile/BaseProjectile.h"


ABaseCharacter::ABaseCharacter()
{
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
	ManagerComp = CreateDefaultSubobject<UManagerComponent>(TEXT("ManagerComp"));


	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterBlockProjectile"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CombatComp && ProjectileClass)
	{
		CombatComp->SetProjectileClass(ProjectileClass);
		CombatComp->GetCombatManage.BindUObject(this, &ThisClass::GetCombatManage);
		CombatComp->GetItemManage.BindUObject(this, &ThisClass::GetItemManage);
		
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	
	
}

void ABaseCharacter::Attack()
{
	if (CombatComp)
	{
		CombatComp->ReadyToFire();
	}
}

void ABaseCharacter::OnHit(AActor* Attacker, const FCombatManage& EnemyCombatManage, const FItemManage& EnemyItemManage)
{
	if (ManagerComp)
	{
		ManagerComp->ReceiveDamage(EnemyCombatManage, EnemyItemManage);
	}
}

FCombatManage ABaseCharacter::GetCombatManage() const
{
	if (ManagerComp)
	{
		return ManagerComp->GetCombatManage();
	}
	return FCombatManage();
}

FItemManage ABaseCharacter::GetItemManage() const
{
	if (ManagerComp)
	{
		return ManagerComp->GetItemManage();
	}
	return FItemManage();
}

void ABaseCharacter::Dead()
{
	GetMovementComponent()->SetActive(false);
}
