// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerCombatComponent.h"

#include "Roguelike/Projectile/BaseProjectile.h"


#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bAttackPressed = false;
	bFireCooldown = false;
	Delay = 0.5f;
	MutliShotTime = 0.2f;
	
}

void UPlayerCombatComponent::ReadyToFire(bool bPressed)
{
	Super::ReadyToFire(bPressed);

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
					MultiShotTimerDelegate.BindUFunction(this, FName("Multishot"), CombatManager, ItemManager);
					GetWorld()->GetTimerManager().SetTimer(MultiShotTimerHandle, MultiShotTimerDelegate, MutliShotTime, false);
				}
				if (HaveItem(ItemManager, EOnceEquippedItem::TRIPLE))
				{
					Triple(CombatManager, ItemManager);
					return;
				}
			}
			Fire(CombatManager, ItemManager);
		}
	}
}

void UPlayerCombatComponent::Fire(const FCombatManager& CombatManager, const FItemManager& ItemManager)
{
	if (bFireCooldown) return;
	bFireCooldown = true;

	if (NormalFire(CombatManager, ItemManager))
	{
		StartFireTimer();
	}
}

void UPlayerCombatComponent::Triple(const FCombatManager& CombatManager, const FItemManager& ItemManager)
{
	if (bFireCooldown) return;
	bFireCooldown = true;

	if (TripleFire(CombatManager, ItemManager))
	{
		StartFireTimer();
	}
}

bool UPlayerCombatComponent::NormalFire(const FCombatManager& CombatManager, const FItemManager& ItemManager)
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
			SpawnedProjectile->SetRange(CombatManager.Range);
			SpawnedProjectile->SetItemManager(ItemManager);
			return true;
		}
	}
	return false;
}

bool UPlayerCombatComponent::TripleFire(const FCombatManager& CombatManager, const FItemManager& ItemManager)
{
	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	if (ProjectileClass && GetWorld())
	{
		float Degree = 90.f;
		for (int32 i = 0; i < 3; ++i)
		{
			ABaseProjectile* SpawnedProjectile =
				GetWorld()->SpawnActor<ABaseProjectile>(
					ProjectileClass,
					GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f,
					FRotator(0.f, 0.f, 0.f),
					Params);
			FVector TripleVector = UKismetMathLibrary::CreateVectorFromYawPitch(GetOwner()->GetActorRotation().Yaw + (Degree -= 45.f), 0.f);
			if (SpawnedProjectile)
			{
				SpawnedProjectile->SetVelocity(TripleVector);
				SpawnedProjectile->SetCombatManage(CombatManager);
				SpawnedProjectile->SetRange(CombatManager.Range);
				SpawnedProjectile->SetItemManager(ItemManager);
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void UPlayerCombatComponent::Multishot(const FCombatManager& CombatManager, const FItemManager& ItemManager)
{
	if (HaveItem(ItemManager, EOnceEquippedItem::TRIPLE))
	{
		TripleFire(CombatManager, ItemManager);
	}
	else
	{
		NormalFire(CombatManager, ItemManager);
	}
}

void UPlayerCombatComponent::StartFireTimer()
{
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ThisClass::FireTimerFinished, Delay);
}

void UPlayerCombatComponent::FireTimerFinished()
{
	bFireCooldown = false;

	if (bAttackPressed)
	{
		ReadyToFire(bAttackPressed);
	}
}

bool UPlayerCombatComponent::HaveItem(const FItemManager& Manage, EOnceEquippedItem ItemType)
{
	return Manage.OnceEquippedItem & static_cast<uint8>(ItemType);
}