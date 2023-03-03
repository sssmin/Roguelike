// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterCombatComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Projectile/BombProjectile.h"
#include "Roguelike/Projectile/OnetoAnotherProjectile.h"
#include "Roguelike/Game/RLGameModeBase.h"

UMonsterCombatComponent::UMonsterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UMonsterCombatComponent::CalcThrowVector(AActor* Target)
{
	if (Target)
	{
		FVector OutVector;
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(
			this,
			OutVector,
			GetOwner()->GetActorLocation(),
			Target->GetActorLocation(),
			GetWorld()->GetGravityZ()
		);
		return OutVector;
	}
	return FVector::ZeroVector;
}

void UMonsterCombatComponent::FireToDir(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType)
{
	PlayFireMontage();
		
	if (GetCombatManager.IsBound())
	{
		FCombatManager CombatManager = GetCombatManager.Execute();
		FActorSpawnParameters Params;

		Params.Owner = GetOwner();
		if (ProjectileClass && GetWorld())
		{
			AMonsterProjectile* SpawnedProjectile =
				GetWorld()->SpawnActor<AMonsterProjectile>(
					ProjectileClass,
					SpawnLoc,
					GetOwner()->GetActorRotation(),
					Params);
			if (SpawnedProjectile)
			{
				SpawnedProjectile->SetVelocity(Dir);
				SpawnedProjectile->SetCombatManage(CombatManager);
				SpawnedProjectile->SetDamageType(DamageType);
			}
		}
	}
}

void UMonsterCombatComponent::ThrowBall(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType)
{
	
}

void UMonsterCombatComponent::FireInParts(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType)
{
	PlayFireMontage();
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
					SpawnedProjectile->SetDamageType(DamageType);
				}
			}
		}
	}
}

void UMonsterCombatComponent::FireSpreadFromCenter(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType)
{
	PlayFireMontage();
		
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
				FVector Dir = UKismetMathLibrary::CreateVectorFromYawPitch(GetOwner()->GetActorRotation().Yaw + (Degree -= DeltaDegree), 0.f);
				AMonsterProjectile* SpawnedProjectile =
					GetWorld()->SpawnActor<AMonsterProjectile>(
						ProjectileClass,
						GetOwner()->GetActorLocation() + Dir * 100.f,
						FRotator(0.f, 0.f, 0.f),
						Params);

				if (SpawnedProjectile)
				{
					SpawnedProjectile->SetVelocity(Dir);
					SpawnedProjectile->SetCombatManage(CombatManager);
					SpawnedProjectile->SetDamageType(DamageType);
				}
			}
		}
	}
}

void UMonsterCombatComponent::ThrowBomb(AActor* Target, TSubclassOf<UDamageType> DamageType)
{
	PlayFireMontage();
		
	if (GetCombatManager.IsBound())
	{
		FCombatManager CombatManager = GetCombatManager.Execute();
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();
		if (BombProjectileClass && GetWorld())
		{
			ABombProjectile* SpawnedProjectile =
				GetWorld()->SpawnActor<ABombProjectile>(
					BombProjectileClass,
					GetOwner()->GetActorLocation(),
					GetOwner()->GetActorRotation(),
					Params);
			if (SpawnedProjectile)
			{
				FVector ThrowVector = CalcThrowVector(Target);
				SpawnedProjectile->SetVelocity(ThrowVector);
				SpawnedProjectile->SetCombatManage(CombatManager);
				SpawnedProjectile->SetDamageType(DamageType);
			}
		}
	}
}

void UMonsterCombatComponent::FireOneToTwo(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType)
{
	PlayFireMontage();
	
	if (GetCombatManager.IsBound())
	{
		FCombatManager CombatManager = GetCombatManager.Execute();
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();

		if (OnetoAnotherProjectileClass && GetWorld())
		{
			float Degree = StartDegree + DeltaDegree;
			for (int32 i = 0; i < Parts; ++i)
			{
				AOnetoAnotherProjectile* SpawnedProjectile =
					GetWorld()->SpawnActor<AOnetoAnotherProjectile>(
						OnetoAnotherProjectileClass,
						GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f,
						FRotator(0.f, 0.f, 0.f),
						Params);

				if (SpawnedProjectile)
				{
					FVector Dir = UKismetMathLibrary::CreateVectorFromYawPitch(SpawnedProjectile->GetActorRotation().Yaw + (Degree -= DeltaDegree), 0.f);
					SpawnedProjectile->SetVelocity(GetOwner()->GetActorForwardVector());
					SpawnedProjectile->SetCombatManage(CombatManager);
					SpawnedProjectile->SetDamageType(DamageType);
					SpawnedProjectile->OneToTwo(Dir);
				}
			}
		}
	}
}

void UMonsterCombatComponent::FireVariousProjectile(int32 Parts, TSubclassOf<UDamageType> DamageType)
{
	PlayFireMontage();
	
	if (GetCombatManager.IsBound())
	{
		FCombatManager CombatManager = GetCombatManager.Execute();
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();
		float Degree = 90;

		TArray<AMonsterProjectile*> SpawnedProjectiles;

		for (int32 i = 0; i < Parts; ++i)
		{
			FVector Dir = UKismetMathLibrary::CreateVectorFromYawPitch(GetOwner()->GetActorRotation().Yaw + (Degree -= 45), 0.f);
			AMonsterProjectile* SpawnedProjectile =
				GetWorld()->SpawnActor<AMonsterProjectile>(
					ProjectileClass,
					GetOwner()->GetActorLocation() + Dir * GetOwner()->GetActorForwardVector() * 100.f,
					FRotator(0.f, 0.f, 0.f),
					Params);
			if (SpawnedProjectile)
			{
				SpawnedProjectile->SetVelocity(GetOwner()->GetActorForwardVector());
				SpawnedProjectile->SetCombatManage(CombatManager);
				SpawnedProjectile->SetDamageType(DamageType);
			}
		}
	}
}

UMonsterCombatComponent* UMonsterCombatComponent::GetMonsterCombatComp(AActor* Character)
{
	if (Character)
	{
		return Character->FindComponentByClass<UMonsterCombatComponent>();
	}
	return nullptr;
}

void UMonsterCombatComponent::RequestHeal()
{
	if (Cast<AMonsterCharacter>(GetOwner())->IsDead()) return;
	if (Cast<AMonsterCharacter>(Turret)) 
	{
		Cast<AMonsterCharacter>(Turret)->RequestHeal(GetOwner());
	}
}
