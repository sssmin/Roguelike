// Fill out your copyright notice in the Description page of Project Settings.
#include "RLGameModeBase.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Character/Monster/MonsterCharacter.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Item/ElementItem.h"
#include "Roguelike/Item/HealItem.h"
#include "Roguelike/Actor/CellActor.h"

ARLGameModeBase::ARLGameModeBase()
{
	MobSpawnPoints.Add(FVector(-300.f, 0.f, 200.f));
	MobSpawnPoints.Add(FVector(0.f, -300.f, 200.f));
	MobSpawnPoints.Add(FVector(300.f, 0.f, 200.f));
	MobSpawnPoints.Add(FVector(0.f, 300.f, 200.f));
	MobSpawnPoints.Add(FVector(-300.f, -300.f, 200.f));
	MobSpawnPoints.Add(FVector(300.f, 300.f, 200.f));
}

void ARLGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARLGameModeBase::SpawnMob(int32 StageLevel, int32 MobCount)
{
	FCombatManager CombatManager;
	FHealthManager HealthManager;
	SetMonsterManager(StageLevel, HealthManager, CombatManager); //out param
	CombatManager = SetRandomElement(StageLevel, CombatManager);
	int32 EliteNum = MobCount / 3;

	//for (int32 i = 0; i < MobCount; ++i)
		for (int32 i = 0; i < 1; ++i)
	{
		int32 RandIndex = FMath::RandRange(0, NormalMonsterClasses.Num() - 1);
		RandIndex = 3; //TEST
		TSubclassOf<AMonsterCharacter> SpawnMonsterClass = NormalMonsterClasses[RandIndex];

		if (GetWorld())
		{
			AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(SpawnMonsterClass, MobSpawnPoints[i], FRotator::ZeroRotator);
			
			if (Mob && Mob->GetManagerComp())
			{
				Mob->SpawnDefaultController();
				if ((EliteNum-- > 0) && (StageLevel > 1))
				{
					Mob->SetMonsterType(EMonsterType::ELITE);
					CombatManager.ATK *= 1.5f;
					HealthManager.MaxHP *= 1.5f;
					HealthManager.CurrentHP = HealthManager.MaxHP;
				}
				Mob->GetManagerComp()->SetManager(HealthManager, CombatManager);
			}
		}
	}
}

void ARLGameModeBase::SpawnBoss(int32 StageLevel)
{
	FCombatManager CombatManager;
	FHealthManager HealthManager;
	SetMonsterManager(StageLevel, HealthManager, CombatManager); //out param
	CombatManager = SetRandomElement(StageLevel, CombatManager);
	
	if (BossMonsterClass && GetWorld())
	{
		AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(BossMonsterClass, MobSpawnPoints[0], FRotator::ZeroRotator);
		if (Mob && Mob->GetManagerComp())
		{
			Mob->SetMonsterType(EMonsterType::BOSS);
			CombatManager.ATK *= 5.f;
			HealthManager.MaxHP *= 5.f;
			HealthManager.CurrentHP = HealthManager.MaxHP;
			Mob->GetManagerComp()->SetManager(HealthManager, CombatManager);
		}
	}
}

void ARLGameModeBase::SpawnCell(int32 CellIndex, uint8 TempWall, int32 Dir)
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

void ARLGameModeBase::SetPlayerLocation(int32 Dir)
{
	FVector PlayerLocation{ FVector::ZeroVector };
	if (Dir != -1)
	{
		PlayerLocation = SpawnedCell->GetPlayerSpawnLocation(Dir);
	}
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter)
	{
		FVector PlayerLoc = PlayerCharacter->GetActorLocation();
		PlayerCharacter->SetActorLocation(FVector(PlayerLocation.X, PlayerLocation.Y, PlayerLoc.Z));
	}
}

void ARLGameModeBase::SpawnHealItem()
{
	if (HealItemClass && GetWorld())
	{
		SpawnedHealItem = GetWorld()->SpawnActor<AHealItem>(HealItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void ARLGameModeBase::SetMonsterManager(int32 StageLevel, OUT FHealthManager& HealthManager, OUT FCombatManager& CombatManager)
{
	FString MonsterStatTablePath = FString(TEXT("/Game/DataTable/MonsterStat"));
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

void ARLGameModeBase::CreateSidePortal()
{
	if (SpawnedCell)
	{
		SpawnedCell->CreateSidePortal();
	}
}

void ARLGameModeBase::CreateCenterPortal()
{
	if (SpawnedCell)
	{
		SpawnedCell->CreateCenterPortal();
	}
}

void ARLGameModeBase::CreatePrevBossPortal()
{
	if (SpawnedCell)
	{
		SpawnedCell->CreatePrevBossPortal();
	}
}

TArray<UItemInfo*> ARLGameModeBase::CreateRandItem()
{
	SelectedItem.Empty();
	FString ItemInfoTablePath = FString(TEXT("/Game/DataTable/ItemInfo"));
	UDataTable* ItemInfoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemInfoTablePath));
	if (ItemInfoTableObject)
	{
		TArray<FItemInfoTable*> Items;
		ItemInfoTableObject->GetAllRows<FItemInfoTable>(TEXT("AllItem"), Items);
		if (!Items.IsEmpty())
		{
			int32 ItemNum = Items.Num();
			UItemInfo* NewItemInfo = nullptr;
			while (true)
			{
				int32 Rand = FMath::RandRange(0, ItemNum - 1);
				
				if (!SelectedItem.IsEmpty())
				{
					if (SelectedItem[0]->ItemName != (Items[Rand]->ItemName))
					{
						NewItemInfo = UItemInfo::ConstructItemInfo(Items[Rand]->ItemsType, Items[Rand]->DetailType, Items[Rand]->ItemName, Items[Rand]->ItemDesc, Items[Rand]->ItemIcon);
						SelectedItem.Add(NewItemInfo);
						break;
					}
					continue;
				}
				NewItemInfo = UItemInfo::ConstructItemInfo(Items[Rand]->ItemsType, Items[Rand]->DetailType, Items[Rand]->ItemName, Items[Rand]->ItemDesc, Items[Rand]->ItemIcon);
				SelectedItem.Add(NewItemInfo);
			}
		}
	}
	return SelectedItem;
}

