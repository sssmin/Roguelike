// Fill out your copyright notice in the Description page of Project Settings.
#include "BossCombatComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Actor/BreathActor.h"
#include "Roguelike/Actor/WhirlwindActor.h"
#include "Roguelike/Projectile/ThrowBallProjectile.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Game/RLMainGameMode.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

UBossCombatComponent::UBossCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	bIsActiveBreath = false;
	SpawnedBreathNum = 0;
	BreathSpawnTime = 0.f;
	BreathDegree = 30.f;
	BreathMaxSpawnNum = 9;
	WhirlwindMaxNum = 6;
	BreathSpawnLocaton = FVector();
}

void UBossCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsActiveBreath && (SpawnedBreathNum < BreathMaxSpawnNum))
	{
		BreathSpawnTime += DeltaTime;
		if (BreathSpawnTime > 0.1f)
		{
			SpawnBreathActor();
			BreathSpawnTime = 0.f;
		}
	}
}

void UBossCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
}

void UBossCombatComponent::SpawnBreathActor()
{
	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = Cast<APawn>(GetOwner());

	FVector Dir = FVector();
	switch (BreathType)
	{
	case EBreathType::RADIAL:
		Dir = UKismetMathLibrary::CreateVectorFromYawPitch(GetOwner()->GetActorRotation().Yaw + BreathDegree, 0.f);
		BreathDegree -= 7.5f;
		break;
	case EBreathType::FORWARD:
		BreathDegree = FMath::RandRange(-7.5f, 7.5f);
		Dir = UKismetMathLibrary::CreateVectorFromYawPitch(GetOwner()->GetActorRotation().Yaw + BreathDegree, 0.f);
		break;
	}

	const FTransform SpawnTransform = FTransform(Dir.Rotation(), BreathSpawnLocaton + Dir * 350.f, FVector(1.f, 1.f, 1.f));

	if (BreathActorClass)
	{
		ABreathActor* BreathActor = GetWorld()->SpawnActor<ABreathActor>(BreathActorClass, SpawnTransform, Params);
		const UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(GetOwner());
		if (BreathActor && ManagerComp)
		{
			BreathActor->SetCombatManager(ManagerComp->GetCombatManager());
		}
	}
	
	SpawnedBreathNum++;

	if (SpawnedBreathNum == BreathMaxSpawnNum)
	{
		DeactivateBreath();
	}
}

void UBossCombatComponent::Breath(EBreathType Type, FVector SpawnLocation)
{
	bIsActiveBreath = true;
	BreathType = Type;
	SetComponentTickEnabled(true);
	BreathSpawnLocaton = SpawnLocation;
}

void UBossCombatComponent::DeactivateBreath()
{
	if (BreathType == EBreathType::RADIAL) BreathDegree = 30.f;
	bIsActiveBreath = false;
	SpawnedBreathNum = 0;
	SetComponentTickEnabled(false);
}

void UBossCombatComponent::Whirlwind()
{
	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = Cast<APawn>(GetOwner());

	for (int32 i = 0; i < WhirlwindMaxNum; ++i)
	{
		FVector SpawnLocation = GetRandomFloor();
		FTransform SpawnTransform = FTransform(GetOwner()->GetActorTransform().GetRotation(), SpawnLocation, FVector(1.f, 1.f, 1.f));
		if (WhirlwindActorClass)
		{
			AWhirlwindActor* WhirlwindActor = GetWorld()->SpawnActor<AWhirlwindActor>(WhirlwindActorClass, SpawnTransform, Params);
			const UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(GetOwner());
			if (WhirlwindActor && ManagerComp)
			{
				SpawnedWhirlwindActors.Add(WhirlwindActor);
				WhirlwindActor->SetCombatManager(ManagerComp->GetCombatManager());
			}
		}
	}
}

void UBossCombatComponent::RemoveSpawnedWhirlwindActor(AWhirlwindActor* WhirlwindActor)
{
	if (SpawnedWhirlwindActors.Contains(WhirlwindActor))
	{
		SpawnedWhirlwindActors.Remove(WhirlwindActor);
	}
}

void UBossCombatComponent::ReserveDestroySpawnedWhirlwind()
{
	for (const auto Actor : SpawnedWhirlwindActors)
    {
    	if (Actor)
    	{
    		Actor->ReserveDestroy();
    	}
    }
}

FVector UBossCombatComponent::GetRandomFloor() const
{
	const float RandValueX = FMath::RandRange(-800.f, 800.f);
	const float RandValueY = FMath::RandRange(-800.f, 800.f);
	const FVector TargetLocation = GetOwner()->GetActorLocation() + FVector(RandValueX, RandValueY, 0.f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TargetLocation, TargetLocation + (GetOwner()->GetActorUpVector() * -600.f), ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		return HitResult.ImpactPoint;
	}
	return FVector::ZeroVector;
}

void UBossCombatComponent::ThrowBall()
{
	const FVector SpawnLoc = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f;
	const FVector Dir = GetOwner()->GetActorForwardVector();
	FireThrowBall(SpawnLoc, Dir, USkillDamageType::StaticClass());
}

void UBossCombatComponent::FireToDir(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType)
{
	if (GetCombatManager.IsBound())
	{
		const FCombatManager CombatManager = GetCombatManager.Execute();
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();
		if (GetProjectileClass() && GetWorld())
		{
			AMonsterProjectile* SpawnedProjectile =
				GetWorld()->SpawnActor<AMonsterProjectile>(
					GetProjectileClass(),
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

void UBossCombatComponent::FireThrowBall(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType) const
{
	if (GetCombatManager.IsBound())
	{
		const FCombatManager CombatManager = GetCombatManager.Execute();
		FActorSpawnParameters Params;

		Params.Owner = GetOwner();
		if (ThrowProjectileClass && GetWorld())
		{
			AThrowBallProjectile* SpawnedProjectile =
				GetWorld()->SpawnActor<AThrowBallProjectile>(
					ThrowProjectileClass,
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

void UBossCombatComponent::Square()
{
	ARLMainGameMode* RLGM = Cast<ARLMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (RLGM)
	{
		FVector CellScale = RLGM->GetBossCellScale();

		for (int32 i = 0; i < 8; ++i)
		{
			FVector SpawnRandLoc = GetRandomLoc(CellScale);
			FVector Dir = GetRandomDir(CellScale, SpawnRandLoc);
			FireToDir(SpawnRandLoc, Dir, USkillDamageType::StaticClass());
		}
	}
}

FVector UBossCombatComponent::GetRandomLoc(const FVector& CellScale) const
{
	const float X = FMath::RandRange(-(CellScale.X / 2) * 0.3, (CellScale.X / 2) * 0.3);
	const float Y = FMath::RandRange(-(CellScale.Y / 2) * 0.3, (CellScale.Y / 2) * 0.3);

	return FVector(X, Y,  GetOwner()->GetActorLocation().Z);
}

FVector UBossCombatComponent::GetRandomDir(const FVector& CellScale, const FVector& RandomLoc)
{
	FVector Dir = FVector::ZeroVector;
	const bool bEither = FMath::RandBool();
	constexpr float CellHalfX = 0;
	constexpr float CellHalfY = 0;

	if (RandomLoc.X > CellHalfX) //-x방향
	{
		if (bEither)
		{
			Dir = FVector(-1.f, 0.f, 0.f);
		}
	}
	if (RandomLoc.X < CellHalfX) //x방향
	{
		if (bEither)
		{
			Dir = FVector(1.f, 0.f, 0.f);
		}
	}
	if (RandomLoc.Y < CellHalfY) //-y방향
	{
		if (!bEither)
		{
			Dir = FVector(0.f, -1.f, 0.f);
		}
	}
	if (RandomLoc.Y > CellHalfY) //y방향
	{
		if (!bEither)
		{
			Dir = FVector(0.f, 1.f, 0.f);
		}
	}
	return Dir;
}