// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Roguelike/Component/ManagerComponent.h"

ABaseCharacter::ABaseCharacter()
{
	ManagerComponent = CreateDefaultSubobject<UManagerComponent>(TEXT("ManagerComp"));

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterBlockProjectile"));
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	OnTakeRadialDamage.AddDynamic(this, &ThisClass::ReceiveDamage);
}

void ABaseCharacter::Tick(float DeltaTime)
{
}

void ABaseCharacter::Attack()
{
	
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

void ABaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	check(ManagerComponent);

	ManagerComponent->ReceiveExplodeDamage(Damage, InstigatedBy, DamageCauser);
}

void ABaseCharacter::Dead()
{
	GetMovementComponent()->SetActive(false);
}
