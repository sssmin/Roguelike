// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Roguelike/Projectile/MonsterProjectile.h"
#include "Roguelike/Projectile/OnetoAnotherProjectile.h"

#include "MonsterCombatComponent.generated.h"

class AMonsterProjectile;
class AOnetoAnotherProjectile;
class ABombProjectile;

UCLASS()
class ROGUELIKE_API UMonsterCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
public:
	UMonsterCombatComponent();
	void FireVariousProjectile(int32 Parts, TSubclassOf<UDamageType> DamageType);
	static UMonsterCombatComponent* GetMonsterCombatComp(AActor* Character);
	void RequestHeal();
	
private:
	FVector CalcThrowVector(AActor* Target);

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterProjectile> ProjectileClass;

	UPROPERTY()
	AActor* Turret;
public:
	template<class T = AMonsterProjectile>
	void FireToDir(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType, TSubclassOf<T> InProjectileClass)
	{
		PlayFireMontage();
		
		if (!InProjectileClass)
		{
			InProjectileClass = ProjectileClass;
		}
		
		if (GetCombatManager.IsBound())
		{
			FCombatManager CombatManager = GetCombatManager.Execute();
			FActorSpawnParameters Params;

			Params.Owner = GetOwner();
			if (InProjectileClass && GetWorld())
			{
				T* SpawnedProjectile =
					GetWorld()->SpawnActor<T>(
						InProjectileClass,
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

	template<class T = AMonsterProjectile>
	void FireInParts(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType, TSubclassOf<T> InProjectileClass)
	{
		PlayFireMontage();
		
		if (!InProjectileClass)
		{
			InProjectileClass = ProjectileClass;
		}
		if (GetCombatManager.IsBound())
		{
			FCombatManager CombatManager = GetCombatManager.Execute();
			FActorSpawnParameters Params;
			Params.Owner = GetOwner();

			if (InProjectileClass && GetWorld())
			{
				float Degree = StartDegree + DeltaDegree;
				for (int32 i = 0; i < Parts; ++i)
				{
					T* SpawnedProjectile =
						GetWorld()->SpawnActor<T>(
							InProjectileClass,
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

	template<class T = AOnetoAnotherProjectile>
	void FireOneToTwo(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType, TSubclassOf<T> InProjectileClass)
	{
		PlayFireMontage();

		if (!InProjectileClass)
		{
			InProjectileClass = ProjectileClass;
		}
		if (GetCombatManager.IsBound())
		{
			FCombatManager CombatManager = GetCombatManager.Execute();
			FActorSpawnParameters Params;
			Params.Owner = GetOwner();

			if (InProjectileClass && GetWorld())
			{
				float Degree = StartDegree + DeltaDegree;
				for (int32 i = 0; i < Parts; ++i)
				{
					T* SpawnedProjectile =
						GetWorld()->SpawnActor<T>(
							InProjectileClass,
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

	template<class T = AMonsterProjectile>
	void FireSpreadFromCenter(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType, TSubclassOf<T> InProjectileClass)
	{
		PlayFireMontage();
		
		if (!InProjectileClass)
		{
			InProjectileClass = ProjectileClass;
		}
		if (GetCombatManager.IsBound())
		{
			FCombatManager CombatManager = GetCombatManager.Execute();
			FActorSpawnParameters Params;
			Params.Owner = GetOwner();

			if (InProjectileClass && GetWorld())
			{
				float Degree = StartDegree + DeltaDegree;
				for (int32 i = 0; i < Parts; ++i)
				{
					FVector Dir = UKismetMathLibrary::CreateVectorFromYawPitch(GetOwner()->GetActorRotation().Yaw + (Degree -= DeltaDegree), 0.f);
					T* SpawnedProjectile =
						GetWorld()->SpawnActor<T>(
							InProjectileClass,
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

	template<class T = ABombProjectile>
	void ThrowBomb(TSubclassOf<T> InProjectileClass, AActor* Target, TSubclassOf<UDamageType> DamageType)
	{
		PlayFireMontage();
		
		if (GetCombatManager.IsBound())
		{
			FCombatManager CombatManager = GetCombatManager.Execute();
			FActorSpawnParameters Params;
			Params.Owner = GetOwner();
			if (InProjectileClass && GetWorld())
			{
				T* SpawnedProjectile =
					GetWorld()->SpawnActor<T>(
						InProjectileClass,
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

	void SetTurret(AActor* Actor) { Turret = Actor; }
};
