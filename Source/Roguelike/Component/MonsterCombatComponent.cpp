// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterCombatComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Roguelike/Projectile/MonsterProjectile.h"



UMonsterCombatComponent::UMonsterCombatComponent()
{
}

void UMonsterCombatComponent::ReadyToFire(bool bPressed)
{
	if (GetCombatManager.IsBound())
	{
		FCombatManager CombatManager = GetCombatManager.Execute();
		Fire(CombatManager);
	}
}

void UMonsterCombatComponent::Fire(const FCombatManager& CombatManager)
{
	FActorSpawnParameters Params;

	Params.Owner = GetOwner();
	if (ProjectileClass && GetWorld())
	{
		AMonsterProjectile* SpawnedProjectile =
			GetWorld()->SpawnActor<AMonsterProjectile>(
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

void UMonsterCombatComponent::FireInParts(int32 Parts, float StartDegree, float DeltaDegree)
{
	if (GetCombatManager.IsBound())
	{
		FCombatManager CombatManager = GetCombatManager.Execute();
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();

		if (ProjectileClass && GetWorld())
		{
			float Degree = StartDegree + DeltaDegree;
			for (int32 i = 0; i < Parts; ++i)
			{
				AMonsterProjectile* SpawnedProjectile =
					GetWorld()->SpawnActor<AMonsterProjectile>(
						ProjectileClass,
						GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f,
						FRotator(0.f, 0.f, 0.f),
						Params);
				FVector Dir = UKismetMathLibrary::CreateVectorFromYawPitch(GetOwner()->GetActorRotation().Yaw + (Degree -= DeltaDegree), 0.f);
				if (SpawnedProjectile)
				{
					SpawnedProjectile->SetVelocity(Dir);
					SpawnedProjectile->SetCombatManage(CombatManager);
				}
			}
		}
	}
}

void UMonsterCombatComponent::FireOneToTwo(int32 Parts, float StartDegree, float DeltaDegree)
{
	if (GetCombatManager.IsBound())
	{
		FCombatManager CombatManager = GetCombatManager.Execute();
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();

		if (ProjectileClass && GetWorld())
		{
			float Degree = StartDegree + DeltaDegree;
			for (int32 i = 0; i < 2; ++i)
			{
				AMonsterProjectile* SpawnedProjectile =
					GetWorld()->SpawnActor<AMonsterProjectile>(
						ProjectileClass,
						GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f,
						FRotator(0.f, 0.f, 0.f),
						Params);

				if (SpawnedProjectile)
				{
					FVector Dir = UKismetMathLibrary::CreateVectorFromYawPitch(SpawnedProjectile->GetActorRotation().Yaw + (Degree -= DeltaDegree), 0.f);
					SpawnedProjectile->SetVelocity(GetOwner()->GetActorForwardVector());
					SpawnedProjectile->SetCombatManage(CombatManager);
					SpawnedProjectile->OneToTwo(Dir);
				}
			}
		}
	}
}
