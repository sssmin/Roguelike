// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameModeBase.h"
#include "Roguelike/Character/MonsterCharacter.h"
#include "Roguelike/Component/ManagerComponent.h"

ARLGameModeBase::ARLGameModeBase()
{
	MobSpawnPoints.Add(FVector(-300.f, 0.f, 0.f));
	MobSpawnPoints.Add(FVector(0.f, -300.f, 0.f));
	MobSpawnPoints.Add(FVector(300.f, 0.f, 0.f));
	MobSpawnPoints.Add(FVector(0.f, 300.f, 0.f));
	MobSpawnPoints.Add(FVector(-300.f, -300.f, 0.f));
	MobSpawnPoints.Add(FVector(300.f, 300.f, 0.f));
}

void ARLGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARLGameModeBase::SpawnMob(int32 StageLevel, int32 MobCount)
{
	FCombatManage CombatManage;
	FHealthManage HealthManage;
	SetMonsterManage(StageLevel, HealthManage, CombatManage); //out param
	uint8 RandValue = FMath::RandRange(static_cast<uint8>(EElement::NONE), static_cast<uint8>(EElement::MAX) - 1);
	int32 EliteNum = MobCount / 3;

	if (StageLevel == 1)
	{
		CombatManage.Element = EElement::NONE;
	}
	else
	{
		CombatManage.Element = static_cast<EElement>(RandValue);
	}

	for (int32 i = 0; i < MobCount; ++i)
	{
		TSubclassOf<AMonsterCharacter> SpawnMonsterClass;
		if ((EliteNum > 0) && (StageLevel > 1))
		{
			SpawnMonsterClass = EliteMonsterClass;
		}
		else
		{
			SpawnMonsterClass = NormalMonsterClass;
		}

		AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(SpawnMonsterClass, MobSpawnPoints[i], FRotator(0.f, 0.f, 0.f));
		if (Mob && Mob->GetManagerComp())
		{
			if ((EliteNum-- > 0) && (StageLevel > 1))
			{
				Mob->SetMonsterType(EMonsterType::ELITE);
				CombatManage.ATK *= 1.5f;
				HealthManage.MaxHP *= 1.5f;
				HealthManage.CurrentHP = HealthManage.MaxHP;
			}
			Mob->GetManagerComp()->SetManager(HealthManage, CombatManage);
		}
	}
}

void ARLGameModeBase::SpawnBoss(int32 StageLevel)
{
	//보스 스폰
	FCombatManage CombatManage;
	FHealthManage HealthManage;
	SetMonsterManage(StageLevel, HealthManage, CombatManage); //out param
	if (BossMonsterClass)
	{
		AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(BossMonsterClass, MobSpawnPoints[0], FRotator(0.f, 0.f, 0.f));
		if (Mob && Mob->GetManagerComp())
		{
			Mob->SetMonsterType(EMonsterType::BOSS);
			CombatManage.ATK *= 5.f;
			HealthManage.MaxHP *= 5.f;
			HealthManage.CurrentHP = HealthManage.MaxHP;
			Mob->GetManagerComp()->SetManager(HealthManage, CombatManage);
		}
	}
	
}

void ARLGameModeBase::SetMonsterManage(int32 StageLevel, OUT FHealthManage& HealthManage, OUT FCombatManage& CombatManage)
{
	FString MonsterStatTablePath = FString(TEXT("/Game/DataTable/MonsterStat"));
	UDataTable* MonsterStatTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *MonsterStatTablePath));
	if (MonsterStatTableObject)
	{
		FMonsterStatTable* Row = nullptr;
		Row = MonsterStatTableObject->FindRow<FMonsterStatTable>(FName(FString::FromInt(StageLevel)), TEXT(""));
		if (Row)
		{
			CombatManage = FCombatManage(Row->AvgATK, EElement::NONE);
			HealthManage = FHealthManage(Row->AvgMaxHP);
		}
	}

}