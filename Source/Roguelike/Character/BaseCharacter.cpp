// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseCharacter.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Roguelike/Actor/DamageWidgetActor.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Widget/DamageWidget.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ManagerComponent = CreateDefaultSubobject<UManagerComponent>(TEXT("ManagerComp"));

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterBlockProjectile"));
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	OnTakeRadialDamage.AddDynamic(this, &ThisClass::OnExplodeHit);

	FActorSpawnParameters Params;
	FTransform SpawnTransform = GetActorTransform();
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Y += FMath::RandRange(-100.f, 100.f);
	SpawnTransform.SetLocation(SpawnLocation);
	const FString DamageBPPath = TEXT("/Game/Blueprints/Actor/Damage/BP_DamageWidgetActor");
	UBlueprint* DamageBP = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *DamageBPPath));
	if (DamageBP && DamageBP->GeneratedClass)
	{
		DamageWidgetActorClass = *DamageBP->GeneratedClass;
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	if (ManagerComponent)
	{
		ManagerComponent->ReceiveDamage(EnemyCombatManager, EnemyItemManager, Attacker, DamageCauser, DamageType);
	}
}

void ABaseCharacter::OnExplodeHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	check(ManagerComponent);
	const FCombatManager EnemyCombatManager = GetCombatManager();
	
	UE_LOG(LogTemp, Warning, TEXT("ExlpodeHit"));
	ManagerComponent->ReceiveDamage(EnemyCombatManager, FItemManager(), DamagedActor, DamageCauser, DamageType->GetClass());
}

FCombatManager ABaseCharacter::GetCombatManager() const
{
	if (ManagerComponent)
	{
		return ManagerComponent->GetCombatManager();
	}
	return FCombatManager();
}

void ABaseCharacter::ShowDamageWidget(float Damage, bool IsCritical)
{
}

void ABaseCharacter::Destroyed()
{
	Super::Destroyed();

}

bool ABaseCharacter::IsDead()
{
	bool Return = false;
	if (ManagerComponent)
	{
		Return = ManagerComponent->IsDead();
	}
	return Return;
}

void ABaseCharacter::Dead()
{
	if (GetMovementComponent() && GetMesh())
	{
		GetMovementComponent()->SetActive(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
