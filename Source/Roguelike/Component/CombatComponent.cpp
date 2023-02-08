// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Roguelike/Projectile/BaseProjectile.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MutliShotTime = 0.5f;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UCombatComponent::HaveItem(const FItemManage& Manage, EOnceEquippedItem ItemType)
{
	return Manage.OnceEquipedItem & static_cast<uint8>(ItemType);
}

void UCombatComponent::ReadyToFire()
{
	if (GetCombatManage.IsBound())
	{
		FCombatManage CombatManage = GetCombatManage.Execute();
		const FItemManage ItemManage = GetItemManage.Execute();

		if (ItemManage.EquippedItemCount > 0) //뭔가 아이템이 있다.
		{
			if (HaveItem(ItemManage, EOnceEquippedItem::RISK_RETURN))
			{
				CombatManage.ATK *= 2.f;
			}
			if (HaveItem(ItemManage, EOnceEquippedItem::MULTI_SHOT))
			{
				FTimerDelegate MultiShotTimerDelegate; 
				MultiShotTimerDelegate.BindUFunction(this, FName("Fire"), CombatManage, ItemManage);
				GetWorld()->GetTimerManager().SetTimer(MultiShotTimerHandle, MultiShotTimerDelegate, MutliShotTime, false);
			}
		}

		Fire(CombatManage, ItemManage);

	}
}


void UCombatComponent::Fire(const FCombatManage& CombatManage, const FItemManage& ItemManage)
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

			SpawnedProjectile->SetCombatManage(CombatManage);
			SpawnedProjectile->SetItemManage(ItemManage);
		}
	}
}
