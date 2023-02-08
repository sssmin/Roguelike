// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameModeBase.h"
#include "Roguelike/Character/MonsterCharacter.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Item/ElementItem.h"
#include "Roguelike/Item/HealItem.h"

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
	CombatManage = SetRandomElement(StageLevel, CombatManage);
	int32 EliteNum = MobCount / 3;

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
		if (GetWorld())
		{
			AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(SpawnMonsterClass, MobSpawnPoints[i], FRotator::ZeroRotator);
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
}

void ARLGameModeBase::SpawnBoss(int32 StageLevel)
{
	//보스 스폰
	FCombatManage CombatManage;
	FHealthManage HealthManage;
	SetMonsterManage(StageLevel, HealthManage, CombatManage); //out param
	CombatManage = SetRandomElement(StageLevel, CombatManage);
	
	if (BossMonsterClass && GetWorld())
	{
		AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(BossMonsterClass, MobSpawnPoints[0], FRotator::ZeroRotator);
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

void ARLGameModeBase::SpawnHealItem()
{
	if (HealItemClass && GetWorld())
	{
		GetWorld()->SpawnActor<AHealItem>(HealItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
		
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
			CombatManage = FCombatManage(Row->AvgATK, EElement::NONE, 2.f);
			HealthManage = FHealthManage(Row->AvgMaxHP);
		}
	}
}

FCombatManage& ARLGameModeBase::SetRandomElement(int32 StageLevel, FCombatManage& CombatManage)
{
	uint8 RandValue = FMath::RandRange(static_cast<uint8>(EElement::NONE), static_cast<uint8>(EElement::MAX) - 1);
	if (StageLevel == 1)
	{
		CombatManage.Element = EElement::NONE;
	}
	else
	{
		CombatManage.Element = static_cast<EElement>(RandValue);
		SpawnCounterElementItem(CombatManage.Element);
	}
	return CombatManage;
}


void ARLGameModeBase::SpawnCounterElementItem(EElement Element)
{
	EElement ItemElement = EElement::NONE;
	switch (Element)
	{
	case EElement::FIRE:
		ItemElement = EElement::WATER;
		break;
	case EElement::WATER:
		ItemElement = EElement::EARTH;
		 break;
	case EElement::EARTH:
		ItemElement = EElement::FIRE;
		 break;
	case EElement::DARKNESS:
		ItemElement = EElement::LIGHT;
		break;
	case EElement::LIGHT:
		ItemElement = EElement::DARKNESS;
		break;
	}

	if (ElementItemClass && GetWorld())
	{
		AElementItem* ElementItem = GetWorld()->SpawnActor<AElementItem>(ElementItemClass);
		if (ElementItem)
		{
			ElementItem->SetElement(ItemElement);
			ElementItem->SetParticle();
		}
	}
	
}