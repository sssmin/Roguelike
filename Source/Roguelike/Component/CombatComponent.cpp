// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Roguelike/Projectile/BaseProjectile.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MutliShotTime = 0.1f;
	bFireCooldown = false;
	bAttackPressed = false;
	Delay = 0.5f;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UCombatComponent::HaveItem(const FItemManager& Manage, EOnceEquippedItem ItemType)
{
	return Manage.OnceEquippedItem & static_cast<uint8>(ItemType);
}

void UCombatComponent::ReadyToFire(bool bPressed)
{
	bAttackPressed = bPressed;

	if (!bAttackPressed) return;

	if (GetCombatManager.IsBound())
	{
		FCombatManager CombatManager = GetCombatManager.Execute();

		if (GetItemManager.IsBound())
		{
			const FItemManager ItemManager = GetItemManager.Execute();

			if (ItemManager.EquippedItemCount > 0) //OnceEquipItem
			{
				if (HaveItem(ItemManager, EOnceEquippedItem::RISK_RETURN))
				{
					CombatManager.ATK *= 2.f;
				}
				if (HaveItem(ItemManager, EOnceEquippedItem::MULTI_SHOT))
				{
					FTimerDelegate MultiShotTimerDelegate;
					MultiShotTimerDelegate.BindUFunction(this, FName("Fire"), CombatManager, ItemManager);
					GetWorld()->GetTimerManager().SetTimer(MultiShotTimerHandle, MultiShotTimerDelegate, MutliShotTime, false);
				}
			}

			Fire(CombatManager, ItemManager); //Player
		}
		else
		{
			Fire(CombatManager); //Monster
		}
	}
}

void UCombatComponent::Fire(const FCombatManager& CombatManager, const FItemManager& ItemManager)
{
	if (bFireCooldown) return;
	FActorSpawnParameters Params;
	bFireCooldown = true;
	
	Params.Owner = GetOwner();
	if (ProjectileClass && GetWorld())
	{
		ABaseProjectile* SpawnedProjectile =
			GetWorld()->SpawnActor<ABaseProjectile>(
				ProjectileClass,
				GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f,
				GetOwner()->GetActorRotation(),
				Params);
		if (SpawnedProjectile)
		{
			SpawnedProjectile->SetVelocity(GetOwner()->GetActorForwardVector());

			SpawnedProjectile->SetCombatManage(CombatManager);
			SpawnedProjectile->SetItemManager(ItemManager);
		}
		StartFireTimer();
	}
}

void UCombatComponent::Fire(const FCombatManager& CombatManager)
{
	FActorSpawnParameters Params;

	Params.Owner = GetOwner();
	if (ProjectileClass && GetWorld())
	{
		ABaseProjectile* SpawnedProjectile =
			GetWorld()->SpawnActor<ABaseProjectile>(
				ProjectileClass,
				GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f,
				GetOwner()->GetActorRotation(),
				Params);
		if (SpawnedProjectile)
		{
			SpawnedProjectile->SetVelocity(GetOwner()->GetActorForwardVector());

			SpawnedProjectile->SetCombatManage(CombatManager);
		}
	}
}

void UCombatComponent::StartFireTimer()
{
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ThisClass::FireTimerFinished, Delay);
}

void UCombatComponent::FireTimerFinished()
{
	bFireCooldown = false;

	if (bAttackPressed)
	{
		ReadyToFire(bAttackPressed);
	}
}