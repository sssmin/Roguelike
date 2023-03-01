// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterCombatComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Projectile/BombProjectile.h"

UMonsterCombatComponent::UMonsterCombatComponent()
{
}

FVector UMonsterCombatComponent::CalcThrowVector(AActor* Target)
{
	FVector OutVector;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		OutVector,
		GetOwner()->GetActorLocation(),
		Target->GetActorLocation(),
		GetWorld()->GetGravityZ()
	);

	/*FPredictProjectilePathParams Params(20.f, GetOwner()->GetActorLocation(), OutVector, 15.f);
	Params.DrawDebugTime = 3.f;
	Params.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
	Params.OverrideGravityZ = GetWorld()->GetGravityZ();
	FPredictProjectilePathResult Result;
	UGameplayStatics::PredictProjectilePath(
		this,
		Params,
		Result
	);*/

	return OutVector;
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
	if (Cast<AMonsterCharacter>(Turret)) 
	{
		Cast<AMonsterCharacter>(Turret)->RequestHeal(GetOwner());
	}
}
