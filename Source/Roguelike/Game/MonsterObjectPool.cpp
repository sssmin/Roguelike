// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterObjectPool.h"

#include "RLGameInstance.h"
#include "RLMainGameMode.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Roguelike/Character/BossMonster/BossMonsterCharacter.h"
#include "Roguelike/Character/NormalMonster/MonsterBossEgo.h"
#include "Roguelike/Component/ManagerComponent.h"

void UMonsterObjectPool::Init()
{
	//모든 Pool에 채우기.
	NormalMonsterInit();
	BossMonsterInit();
	BossEgoInit();
}

AMonsterCharacter* UMonsterObjectPool::GetNormalMonster(EKindOfMonster KindOfMonster)
{
	switch (KindOfMonster)
	{
	case EKindOfMonster::Bee:
		return GetDeactivateMonster(BeePool);
	case EKindOfMonster::Dino:
		return GetDeactivateMonster(DinoPool);
	case EKindOfMonster::PrimeHelix:
		return GetDeactivateMonster(PrimeHelixPool);
	case EKindOfMonster::Skeleton:
		return GetDeactivateMonster(SkeletonPool);
	case EKindOfMonster::Suicide:
		return GetDeactivateMonster(SuicidePool);
	case EKindOfMonster::Turret:
		return TurretPool;
	case EKindOfMonster::White:
		return GetDeactivateMonster(BeePool);
	case EKindOfMonster::Wizard:
		return GetDeactivateMonster(WizardPool);
	}
	return nullptr;
}

AMonsterCharacter* UMonsterObjectPool::GetDeactivateMonster(TArray<AMonsterCharacter*> Pool)
{
	for (AMonsterCharacter* Monster : Pool)
	{
		if (!Monster->IsActive()) return Monster;
	}
	return nullptr;
}

AMonsterCharacter* UMonsterObjectPool::GetBossMonster(EKindOfBossMonster KindOfMonster)
{
	switch (KindOfMonster)
	{
	case EKindOfBossMonster::Usurper:
		return UsurperPool;
	case EKindOfBossMonster::SoulEater:
		return SoulEaterPool;
	case EKindOfBossMonster::TerrorBringer:
		return TerrorBringerPool;
	}
	return nullptr;
}

void UMonsterObjectPool::NormalMonsterInit()
{
	ARLBaseGameMode* RLGM = Cast<ARLBaseGameMode>(UGameplayStatics::GetGameMode(this));
	
	if (RLGM)
	{
		for (int32 i = 0; i < static_cast<int32>(EKindOfMonster::MAX) - 1; ++i)
		{
			TSubclassOf<AMonsterCharacter> MonsterClass = RLGM->GetNormalMonsterClass(static_cast<EKindOfMonster>(i));
			if (MonsterClass)
			{
				for (int32 j = 0; j < PoolSize; ++j)
				{FActorSpawnParameters Params;
					Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(MonsterClass, FVector(0.f, 0.f, -300.f), FRotator::ZeroRotator, Params);

					if (Mob)
					{
						Mob->SetActive(false);
						switch (static_cast<EKindOfMonster>(i))
						{
						case EKindOfMonster::Bee:
							BeePool.Add(Mob);
							break;
						case EKindOfMonster::Dino:
							DinoPool.Add(Mob);
							break;
						case EKindOfMonster::PrimeHelix:
							PrimeHelixPool.Add(Mob);
							break;
						case EKindOfMonster::Skeleton:
							SkeletonPool.Add(Mob);
							break;
						case EKindOfMonster::Suicide:
							SuicidePool.Add(Mob);
							break;
						case EKindOfMonster::Turret:
							TurretPool = Mob;
							break;
						case EKindOfMonster::White:
							WhitePool.Add(Mob);
							break;
						case EKindOfMonster::Wizard:
							WizardPool.Add(Mob);
							break;
						}
						if (static_cast<EKindOfMonster>(i) == EKindOfMonster::Turret)
							break;
					}
				}
			}
		}
	}
}

void UMonsterObjectPool::BossMonsterInit()
{
	ARLMainGameMode* RLGM = Cast<ARLMainGameMode>(UGameplayStatics::GetGameMode(this));
	
	if (RLGM)
	{
		for (int32 i = 0; i < static_cast<int32>(EKindOfBossMonster::MAX); ++i)
		{
			TSubclassOf<ABossMonsterCharacter> MonsterClass = RLGM->GetBossMonsterClass(static_cast<EKindOfBossMonster>(i));
			if (MonsterClass)
			{
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				ABossMonsterCharacter* Mob = GetWorld()->SpawnActor<ABossMonsterCharacter>(MonsterClass, FVector(0.f, 0.f, -300.f), FRotator::ZeroRotator, Params);

				if (Mob)
				{
					Mob->SetActive(false);
					switch (static_cast<EKindOfBossMonster>(i))
					{
					case EKindOfBossMonster::Usurper:
						UsurperPool = Mob;
						break;
					case EKindOfBossMonster::SoulEater:
						SoulEaterPool = Mob;
						break;
					case EKindOfBossMonster::TerrorBringer:
						TerrorBringerPool = Mob;
						break;
					}
				}
			}
		}
	}
}

void UMonsterObjectPool::BossEgoInit()
{
	ARLMainGameMode* RLGM = Cast<ARLMainGameMode>(UGameplayStatics::GetGameMode(this));

	if (RLGM)
	{
		TSubclassOf<AMonsterBossEgo> MonsterClass = RLGM->GetBossEgoClass();
		if (MonsterClass)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(MonsterClass, FVector(0.f, 0.f, -300.f), FRotator::ZeroRotator, Params);

			if (Mob)
			{
				Mob->SetActive(false);
				BossEgoPool = Mob;
			}
		}
	}
}
