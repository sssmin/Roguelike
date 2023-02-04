// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
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

void ABaseCharacter::OnHit(AActor* Attacker, const FCombatManage& EnemyCombatManage)
{
	if (ManagerComp)
	{
		ManagerComp->ReceiveDamage(EnemyCombatManage);
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
