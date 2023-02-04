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

	if (StageLevel == 1) //1stage에서는 무속성, 일반몹만
	{
		CombatManage.Element = EElement::NONE;
	}
	else
	{
		CombatManage.Element = static_cast<EElement>(RandValue);
	}

	for (int32 i = 0; i < MobCount; ++i)
	{
		AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(MonsterClass, MobSpawnPoints[i], FRotator(0.f, 0.f, 0.f));
		if (Mob && Mob->GetManagerComp())
		{
			Mob->GetManagerComp()->SetManager(HealthManage, CombatManage);
			//스테이지 레벨에 따라 평균체력,평균공격력이 있다.
			//노말몹이면 평균을 다르고, 정예몹은 1.5배. 몹의 원소는 랜덤으로 하나만 정해서 모두 같게
			//

		}
	}
	
}

void ARLGameModeBase::SpawnBoss(int32 StageLevel)
{
	//보스 스폰
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