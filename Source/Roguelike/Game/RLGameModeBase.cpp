// Fill out your copyright notice in the Description page of Project Settings.
#include "RLGameModeBase.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Character/BossMonster/BossMonsterCharacter.h"
#include "Roguelike/Character/NormalMonster/MonsterBossEgo.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Component/MonsterCombatComponent.h"
#include "Roguelike/Item/ElementItem.h"
#include "Roguelike/Item/HealItem.h"
#include "Roguelike/Actor/CellActor.h"

ARLGameModeBase::ARLGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	MobSpawnPoints.Add(FVector(-300.f, 0.f, 200.f));
	MobSpawnPoints.Add(FVector(0.f, -300.f, 200.f));
	MobSpawnPoints.Add(FVector(300.f, 0.f, 200.f));
	MobSpawnPoints.Add(FVector(0.f, 300.f, 200.f));
	MobSpawnPoints.Add(FVector(-300.f, -300.f, 200.f));
	MobSpawnPoints.Add(FVector(300.f, 300.f, 200.f));
	MobSpawnPoints.Add(FVector(80.f, 80.f, 200.f));
}

void ARLGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void ARLGameModeBase::RequestSpawnMob(int32 StageLevel, int32 MobCount)
{
	FCombatManager CombatManager;
	FHealthManager HealthManager;
	SetMonsterManager(StageLevel, HealthManager, CombatManager); //out param
	CombatManager = SetRandomElement(StageLevel, CombatManager);

	AMonsterCharacter* Turret = nullptr;
	TArray<AMonsterCharacter*> SpawnedMobs;

	// Turret = SpawnMob(1, EKindOfMonster::TURRET, CombatManager, HealthManager)[0]; 	//≈Õ∑ø
	//
	// for (int32 i = 0; i < 2; ++i)
	// {
	// 	const EKindOfMonster RandMonster = static_cast<EKindOfMonster>(FMath::RandRange(1, static_cast<uint8>(EKindOfMonster::MAX) - 1));
	// 	SpawnedMobs.Append(SpawnMob(MobCount / 2, RandMonster, CombatManager, HealthManager));
	// }
	//
	// ConnectTurret(Turret, SpawnedMobs);
	SpawnBoss(EKindOfBossMonster::USURPER, CombatManager, HealthManager); //TEST
}

TArray<AMonsterCharacter*> ARLGameModeBase::SpawnMob(int32 MobCount, EKindOfMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager)
{
	TArray<AMonsterCharacter*> SpawnedMobs;
	TSubclassOf<AMonsterCharacter> SpawnMonsterClass = GetNormalMonsterClass(KindOfMonster); 
	//SpawnMonsterClass = GetNormalMonsterClass(EKindOfMonster::BEE); //TEST
	for (int32 i = 0; i < MobCount; ++i)
	{
		AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(SpawnMonsterClass, MobSpawnPoints[i], FRotator::ZeroRotator);
		if (Mob && Mob->GetManagerComp())
		{
			Mob->SpawnDefaultController();
			Mob->GiveBTToController();
			if (FMath::RandBool())
			{
				Mob->SetMonsterType(EMonsterType::ELITE);
				CombatManager.ATK *= 1.5f;
				HealthManager.MaxHP *= 1.5f;
				HealthManager.CurrentHP = HealthManager.MaxHP;
			}
			Mob->SetKindOfMonster(KindOfMonster);
			Mob->GetManagerComp()->SetManager(HealthManager, CombatManager);
			SpawnedMobs.Add(Mob);
		}
	}
	return SpawnedMobs;
}

void ARLGameModeBase::RequestSpawnBoss(int32 StageLevel)
{
	FCombatManager CombatManager;
	FHealthManager HealthManager;
	SetMonsterManager(StageLevel, HealthManager, CombatManager); //out param
	CombatManager = SetRandomElement(StageLevel, CombatManager);

	EKindOfBossMonster RandMonster = static_cast<EKindOfBossMonster>(FMath::RandRange(0, static_cast<uint8>(EKindOfBossMonster::MAX) - 1));
	RandMonster = EKindOfBossMonster::USURPER; //TEST
	SpawnBoss(RandMonster, CombatManager, HealthManager);
}

void ARLGameModeBase::SpawnBoss(EKindOfBossMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager)
{
	const TSubclassOf<ABossMonsterCharacter> BossClass = GetBossMonsterClass(KindOfMonster);

	AMonsterCharacter* Boss = GetWorld()->SpawnActor<AMonsterCharacter>(BossClass, MobSpawnPoints[6], FRotator::ZeroRotator);
	if (Boss && Boss->GetManagerComp())
	{
		Boss->SetMonsterType(EMonsterType::BOSS);
		Boss->SpawnDefaultController();
		Boss->GiveBTToController();
		CombatManager.ATK *= 5.f;
		HealthManager.MaxHP *= 10.f;
		HealthManager.CurrentHP = HealthManager.MaxHP;
		Boss->GetManagerComp()->SetManager(HealthManager, CombatManager);
		Cast<ABossMonsterCharacter>(Boss)->SetKindOfBossMonster(KindOfMonster);
		AMonsterCharacter* Ego = SpawnBossEgo(CombatManager, Boss);
		if (Ego)
		{
			Cast<ABossMonsterCharacter>(Boss)->SetBossEgo(Ego);
		}
	}
}

AMonsterCharacter* ARLGameModeBase::SpawnBossEgo(const FCombatManager& CombatManager, AMonsterCharacter* Boss)
{
	AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(BossEgoClass, MobSpawnPoints[5], FRotator::ZeroRotator);
	
	if (Mob && Mob->GetManagerComp())
	{
		Mob->SetMonsterType(EMonsterType::EGO);
		Mob->SpawnDefaultController();
		Mob->GiveBTToController();
		Cast<AMonsterBossEgo>(Mob)->SetBoss(Boss);
		
		Mob->SetKindOfMonster(EKindOfMonster::DINO);
		Mob->GetManagerComp()->SetManager(FHealthManager(), CombatManager);
	}
	return Mob;
}

void ARLGameModeBase::ConnectTurret(AMonsterCharacter* Turret, TArray<AMonsterCharacter*> SpawnedMobs)
{
	for (auto Mob : SpawnedMobs)
	{
		if (Mob->GetMonsterCombatComp())
		{
			Mob->GetMonsterCombatComp()->SetTurret(Turret);
		}
	}
}

TSubclassOf<AMonsterCharacter> ARLGameModeBase::GetNormalMonsterClass(EKindOfMonster KindOfMonster) const
{
	const FString NormalMonsterTablePath = FString(TEXT("/Game/DataTable/NormalMonsterClass"));
	UDataTable* NormalMonsterTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *NormalMonsterTablePath));
	if (NormalMonsterTableObject)
	{
		const FString RowName = FString::FromInt(static_cast<uint8>(KindOfMonster));
		FNormalMonsterClassTable* FindRow = NormalMonsterTableObject->FindRow<FNormalMonsterClassTable>(FName(RowName), "");
		if (FindRow)
		{
			return FindRow->MonsterClass;
		}
	}
	return TSubclassOf<AMonsterCharacter>();
}

TSubclassOf<ABossMonsterCharacter> ARLGameModeBase::GetBossMonsterClass(EKindOfBossMonster KindOfMonster) const
{
	const FString BossMonsterTablePath = FString(TEXT("/Game/DataTable/BossMonsterClass"));
	UDataTable* BossMonsterTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *BossMonsterTablePath));
	if (BossMonsterTableObject)
	{
		FString RowName = FString::FromInt(static_cast<uint8>(KindOfMonster));
		FBossMonsterClassTable* FindRow = BossMonsterTableObject->FindRow<FBossMonsterClassTable>(FName(RowName), "");
		if (FindRow)
		{
			return FindRow->MonsterClass;
		}
	}

	return TSubclassOf<ABossMonsterCharacter>();
}

void ARLGameModeBase::RequestSpawnCell(int32 CellIndex, uint8 TempWall, int32 Dir)
{
	DestroySpawnedActors();

	if (CellClasses.IsValidIndex(CellIndex))
	{
		SpawnedCell = GetWorld()->SpawnActor<ACellActor>(CellClasses[CellIndex], FVector::ZeroVector, FRotator::ZeroRotator);
		if (SpawnedCell)
		{
			SpawnedCell->SetTempWall(CellIndex, TempWall);
			SpawnedCell->CreateWall();

			SetPlayerLocation(Dir);
		}
	}
}

void ARLGameModeBase::DestroySpawnedActors()
{
	if (SpawnedElementItem)
	{
		SpawnedElementItem->Destroy();
		SpawnedElementItem = nullptr;
	}
	if (SpawnedCell)
	{
		SpawnedCell->Destroy();
		SpawnedCell = nullptr;
	}
	if (SpawnedHealItem)
	{
		SpawnedHealItem->Destroy();
		SpawnedHealItem = nullptr;
	}
}


void ARLGameModeBase::SetPlayerLocation(int32 Dir) const
{
	FVector PlayerLocation{ FVector::ZeroVector };
	if (Dir != -1)
	{
		PlayerLocation = SpawnedCell->GetPlayerSpawnLocation(Dir);
	}
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter)
	{
		const FVector PlayerLoc = PlayerCharacter->GetActorLocation();
		PlayerCharacter->SetActorLocation(FVector(PlayerLocation.X, PlayerLocation.Y, PlayerLoc.Z));
	}
}

void ARLGameModeBase::RequestSpawnHealItem()
{
	if (HealItemClass && GetWorld())
	{
		SpawnedHealItem = GetWorld()->SpawnActor<AHealItem>(HealItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void ARLGameModeBase::SetMonsterManager(int32 StageLevel, OUT FHealthManager& HealthManager, OUT FCombatManager& CombatManager) const
{
	const FString MonsterStatTablePath = FString(TEXT("/Game/DataTable/MonsterStat"));
	UDataTable* MonsterStatTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *MonsterStatTablePath));
	if (MonsterStatTableObject)
	{
		FMonsterStatTable* Row = nullptr;
		Row = MonsterStatTableObject->FindRow<FMonsterStatTable>(FName(FString::FromInt(StageLevel)), TEXT(""));
		if (Row)
		{
			CombatManager = FCombatManager(Row->AvgATK, EElement::NONE);
			HealthManager = FHealthManager(Row->AvgMaxHP);
		}
	}
}

FCombatManager& ARLGameModeBase::SetRandomElement(int32 StageLevel, FCombatManager& CombatManager)
{
	uint8 RandValue = FMath::RandRange(static_cast<uint8>(EElement::FIRE), static_cast<uint8>(EElement::MAX) - 1);
	if (StageLevel == 1)
	{
		CombatManager.Element = EElement::NONE;
	}
	else
	{
		CombatManager.Element = static_cast<EElement>(RandValue);
		SpawnCounterElementItem(CombatManager.Element);
	}
	return CombatManager;
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
		SpawnedElementItem = GetWorld()->SpawnActor<AElementItem>(ElementItemClass);
		if (SpawnedElementItem)
		{
			SpawnedElementItem->SetElement(ItemElement);
			SpawnedElementItem->SetParticle();
		}
	}
}

void ARLGameModeBase::CreateSidePortal() const
{
	if (SpawnedCell)
	{
		SpawnedCell->CreateSidePortal();
	}
}

void ARLGameModeBase::CreateCenterPortal() const
{
	if (SpawnedCell)
	{
		SpawnedCell->CreateCenterPortal();
	}
}

void ARLGameModeBase::CreatePrevBossPortal() const
{
	if (SpawnedCell)
	{
		SpawnedCell->CreatePrevBossPortal();
	}
}

TArray<UItemInfo*> ARLGameModeBase::CreateRandItem()
{
	SelectedItem.Empty();

	if (ItemInfos.IsEmpty()) InitItemInfoFromTable();
	
	while (true)
	{
		const int32 Rand = FMath::RandRange(0, ItemInfos.Num() - 1);

		if (!SelectedItem.IsEmpty())
		{
			if (SelectedItem[0]->ItemName != (ItemInfos[Rand]->ItemName))
			{
				SelectedItem.Add(ItemInfos[Rand]);
				break;
			}
			continue;
		}
		SelectedItem.Add(ItemInfos[Rand]);
	}
	return SelectedItem;
}

void ARLGameModeBase::InitItemInfoFromTable()
{
	if (ItemInfos.IsEmpty())
	{
		const FString ItemInfoTablePath = FString(TEXT("/Game/DataTable/ItemInfo"));
		UDataTable* ItemInfoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemInfoTablePath));
		if (ItemInfoTableObject)
		{
			TArray<FItemInfoTable*> Items;
			ItemInfoTableObject->GetAllRows<FItemInfoTable>(TEXT("AllItem"), Items);
			for (auto ItemInfo : Items)
			{
				ItemInfos.Add(UItemInfo::ConstructItemInfo(ItemInfo->ItemsType, ItemInfo->DetailType, ItemInfo->ItemName, ItemInfo->ItemDesc, ItemInfo->ItemIcon));
			}
		}
	}
}

FVector ARLGameModeBase::GetBossCellScale() const
{
	if (SpawnedCell)
	{
		return SpawnedCell->GetPlaneScale();
	}
	return FVector();
}

