// Fill out your copyright notice in the Description page of Project Settings.
#include "RLMainGameMode.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Character/BossMonster/BossMonsterCharacter.h"
#include "Roguelike/Character/NormalMonster/MonsterBossEgo.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Component/MonsterCombatComponent.h"
#include "Roguelike/Item/ElementItem.h"
#include "Roguelike/Actor/CellActor.h"
#include "MonsterObjectPool.h"

ARLMainGameMode::ARLMainGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void ARLMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitImage();
	MonsterObjectPool = NewObject<UMonsterObjectPool>(this);
}

void ARLMainGameMode::RequestSpawnMob(int32 StageLevel, int32 MobCount)
{
	FCombatManager CombatManager;
	FHealthManager HealthManager;
	SetMonsterManager(StageLevel, HealthManager, CombatManager); //out param
	CombatManager = SetRandomElement(StageLevel, CombatManager);

	AMonsterCharacter* Turret = nullptr;
	TArray<AMonsterCharacter*> SpawnedMobs;

	Turret = SpawnMob(1, EKindOfMonster::Turret, CombatManager, HealthManager)[0]; //터렛
	
	for (int32 i = 0; i < 2; ++i)
	{
		const EKindOfMonster RandMonster = static_cast<EKindOfMonster>(FMath::RandRange(1, static_cast<uint8>(EKindOfMonster::MAX) - 2));
		SpawnedMobs.Append(SpawnMob(MobCount / 2, RandMonster, CombatManager, HealthManager));
	}
	
	ConnectTurret(Turret, SpawnedMobs);
}

TArray<AMonsterCharacter*> ARLMainGameMode::SpawnMob(int32 MobCount, EKindOfMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager)
{
	TArray<AMonsterCharacter*> SpawnedMobs;
	if (MonsterObjectPool)
	{
		for (int32 i = 0; i < MobCount; ++i)
		{
			AMonsterCharacter* Mob = MonsterObjectPool->GetNormalMonster(KindOfMonster);
			if (Mob && Mob->GetManagerComp())
			{
				if (FMath::RandBool())
				{
					Mob->SetMonsterType(EMonsterType::Elite);
					CombatManager.ATK *= 1.5f;
					HealthManager.MaxHP *= 1.5f;
					HealthManager.CurrentHP = HealthManager.MaxHP;
				}
				Mob->SetKindOfMonster(KindOfMonster);
				Mob->SpawnInit(HealthManager, CombatManager, GetMobSpawnPoints()[i]);
				
				SpawnedMobs.Add(Mob);
			}
		}
	}
	return SpawnedMobs;
}

void ARLMainGameMode::RequestSpawnBoss(int32 StageLevel)
{
	FCombatManager CombatManager;
	FHealthManager HealthManager;
	SetMonsterManager(StageLevel, HealthManager, CombatManager); //out param
	CombatManager = SetRandomElement(StageLevel, CombatManager);

	EKindOfBossMonster RandMonster = static_cast<EKindOfBossMonster>(FMath::RandRange(0, static_cast<uint8>(EKindOfBossMonster::MAX) - 1));
	SpawnBoss(RandMonster, CombatManager, HealthManager);
}

void ARLMainGameMode::SpawnBoss(EKindOfBossMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager)
{
	if (MonsterObjectPool)
	{
		AMonsterCharacter* Boss = MonsterObjectPool->GetBossMonster(KindOfMonster);
		if (Boss && Boss->GetManagerComp())
		{
			CombatManager.ATK *= 5.f;
			HealthManager.MaxHP *= 10.f;
			HealthManager.CurrentHP = HealthManager.MaxHP;
			Boss->SetMonsterType(EMonsterType::Boss);
			Cast<ABossMonsterCharacter>(Boss)->SetKindOfBossMonster(KindOfMonster);
			Boss->SpawnInit(HealthManager, CombatManager, GetMobSpawnPoints()[6]);
			
			AMonsterCharacter* Ego = SpawnBossEgo(CombatManager, Boss);
			if (Ego)
			{
				Cast<ABossMonsterCharacter>(Boss)->SetBossEgo(Ego);
			}
		}
	}
}

AMonsterCharacter* ARLMainGameMode::SpawnBossEgo(const FCombatManager& CombatManager, AMonsterCharacter* Boss)
{
	AMonsterCharacter* Mob = MonsterObjectPool->GetBossEgoMonster();
	
	if (Mob && Mob->GetManagerComp())
	{
		Mob->SetMonsterType(EMonsterType::Ego);
		Mob->SetKindOfMonster(EKindOfMonster::Dino);
		
		Cast<AMonsterBossEgo>(Mob)->SetBoss(Boss);
		Mob->SpawnInit(FHealthManager(), CombatManager, GetMobSpawnPoints()[5]);
	}
	return Mob;
}

void ARLMainGameMode::InitImage()
{
	if (StateImages.IsEmpty())
	{
		FString StateImageTablePath = FString(TEXT("/Game/DataTable/StateImage"));
		UDataTable* StateImageTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *StateImageTablePath));
		if (StateImageTableObject)
		{
			TArray<FName> RowNames = StateImageTableObject->GetRowNames();

			for (auto& RowName : RowNames)
			{
				const FStateImageTable* FindRow = StateImageTableObject->FindRow<FStateImageTable>(RowName, "");
				if (FindRow)
				{
					StateImages.Add(FindRow->State, FindRow->Image);
				}
			}
		}
	}
	if (BuffImages.IsEmpty())
	{
		FString BuffImageTablePath = FString(TEXT("/Game/DataTable/BuffImage"));
		UDataTable* BuffImageTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *BuffImageTablePath));
		if (BuffImageTableObject)
		{
			TArray<FName> RowNames = BuffImageTableObject->GetRowNames();

			for (auto& RowName : RowNames)
			{
				const FBuffImageTable* FindRow = BuffImageTableObject->FindRow<FBuffImageTable>(RowName, "");
				if (FindRow)
				{
					BuffImages.Add(FindRow->Buff, FindRow->Image);
				}
			}
		}
	}
	if (StatImages.IsEmpty())
	{
		FString StatImageTablePath = FString(TEXT("/Game/DataTable/StatImage"));
		UDataTable* StatImageTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *StatImageTablePath));
		if (StatImageTableObject)
		{
			TArray<FName> RowNames = StatImageTableObject->GetRowNames();

			for (auto& RowName : RowNames)
			{
				const FStatImageTable* FindRow = StatImageTableObject->FindRow<FStatImageTable>(RowName, "");
				if (FindRow)
				{
					StatImages.Add(FindRow->ItemType, FindRow->Image);
				}
			}
		}
	}
}

UTexture2D* ARLMainGameMode::GetStateImage(EState State)
{
	if (StateImages.Contains(State)) return *StateImages.Find(State);
	return nullptr;
}

UTexture2D* ARLMainGameMode::GetBuffImage(EBuff Buff)
{
	if (BuffImages.Contains(Buff)) return *BuffImages.Find(Buff);
	return nullptr;
}

UTexture2D* ARLMainGameMode::GetStatImage(EINFStackItem StatItem)
{
	if (StatImages.Contains(StatItem)) return *StatImages.Find(StatItem);
	return nullptr;
}

void ARLMainGameMode::ConnectTurret(AMonsterCharacter* Turret, TArray<AMonsterCharacter*> SpawnedMobs)
{
	for (auto Mob : SpawnedMobs)
	{
		if (Mob->GetMonsterCombatComp())
		{
			Mob->GetMonsterCombatComp()->SetTurret(Turret);
		}
	}
}

TSubclassOf<ABossMonsterCharacter> ARLMainGameMode::GetBossMonsterClass(EKindOfBossMonster KindOfMonster) const
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

void ARLMainGameMode::RequestSpawnCell(int32 CellIndex, uint8 TempWall, uint8 Dir)
{
	DestroySpawnedActors();

	if (GetCellClasses().IsValidIndex(CellIndex))
	{
		SetSpawnedCell(GetWorld()->SpawnActor<ACellActor>(GetCellClasses()[CellIndex], FVector::ZeroVector, FRotator::ZeroRotator));
		if (GetSpawnedCell())
		{
			GetSpawnedCell()->SetTempWall(CellIndex, TempWall);
			GetSpawnedCell()->CreateWall();

			SetPlayerLocation(Dir);
		}
	}
}

void ARLMainGameMode::DestroySpawnedActors()
{
	Super::DestroySpawnedActors();

	if (SpawnedElementItem)
	{
		SpawnedElementItem->Destroy();
		SpawnedElementItem = nullptr;
	}
}

void ARLMainGameMode::SetPlayerLocation(uint8 Dir) const
{
	FVector PlayerLocation{ FVector::ZeroVector };
	if ((Dir != -1) && (static_cast<EDir>(Dir) != EDir::Load))
	{
		PlayerLocation = GetSpawnedCell()->GetPlayerSpawnLocation(Dir);
	}
	if (static_cast<EDir>(Dir) != EDir::Load)
	{
		if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			const FVector PlayerLoc = PlayerCharacter->GetActorLocation();
			PlayerCharacter->SetActorLocation(FVector(PlayerLocation.X, PlayerLocation.Y, PlayerLoc.Z));
		}
	}
}

void ARLMainGameMode::RequestSpawnHealItem()
{
	Super::RequestSpawnHealItem();
}

void ARLMainGameMode::SetMonsterManager(int32 StageLevel, OUT FHealthManager& HealthManager, OUT FCombatManager& CombatManager) const
{
	const FString MonsterStatTablePath = FString(TEXT("/Game/DataTable/MonsterStat"));
	UDataTable* MonsterStatTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *MonsterStatTablePath));
	if (MonsterStatTableObject)
	{
		FMonsterStatTable* Row = nullptr;
		Row = MonsterStatTableObject->FindRow<FMonsterStatTable>(FName(FString::FromInt(StageLevel)), TEXT(""));
		if (Row)
		{
			CombatManager = FCombatManager(Row->AvgATK, EElement::None);
			HealthManager = FHealthManager(Row->AvgMaxHP);
		}
	}
}

FCombatManager& ARLMainGameMode::SetRandomElement(int32 StageLevel, FCombatManager& CombatManager)
{
	uint8 RandValue = FMath::RandRange(static_cast<uint8>(EElement::Fire), static_cast<uint8>(EElement::MAX) - 1);
	if (StageLevel == 1)
	{
		CombatManager.Element = EElement::None;
	}
	else
	{
		CombatManager.Element = static_cast<EElement>(RandValue);
		SpawnCounterElementItem(CombatManager.Element);
	}
	return CombatManager;
}

void ARLMainGameMode::SpawnCounterElementItem(EElement Element)
{
	EElement ItemElement = EElement::None;
	switch (Element)
	{
	case EElement::Fire:
		ItemElement = EElement::Water;
		break;
	case EElement::Water:
		ItemElement = EElement::Earth;
		break;
	case EElement::Earth:
		ItemElement = EElement::Fire;
		break;
	case EElement::Darkness:
		ItemElement = EElement::Light;
		break;
	case EElement::Light:
		ItemElement = EElement::Darkness;
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

void ARLMainGameMode::CreateSidePortal() const
{
	Super::CreateSidePortal();
}

void ARLMainGameMode::CreateCenterPortal() const
{
	Super::CreateCenterPortal();
}

void ARLMainGameMode::CreatePrevBossPortal() const
{
	if (GetSpawnedCell())
	{
		GetSpawnedCell()->CreatePrevBossPortal();
	}
}

TArray<UItemInfo*> ARLMainGameMode::CreateLoadItem()
{
	if (GetItemInfos().IsEmpty()) InitItemInfoFromTable();

	return TArray<UItemInfo*>();
}

FVector ARLMainGameMode::GetBossCellScale() const
{
	if (GetSpawnedCell())
	{
		return GetSpawnedCell()->GetPlaneScale();
	}
	return FVector();
}

void ARLMainGameMode::SetItemIcon(TArray<UItemInfo*>& InItemInfos)
{
	if (GetItemInfos().IsEmpty()) InitItemInfoFromTable();

	for (auto& ItemInfo : InItemInfos)
	{
		for (const auto& SavedInfo : GetItemInfos())
		{
			if (SavedInfo->ItemName == ItemInfo->ItemName)
			{
				ItemInfo->ItemIcon = SavedInfo->ItemIcon;
			}
		}
	}
}

void ARLMainGameMode::InitMonsterObjectPool()
{
	if (MonsterObjectPool)
	{
		MonsterObjectPool->Init();
	}
}