// Fill out your copyright notice in the Description page of Project Settings.
#include "RLBaseGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Item/ElementItem.h"
#include "Roguelike/Item/HealItem.h"
#include "Roguelike/Actor/CellActor.h"

ARLBaseGameMode::ARLBaseGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	MobSpawnPoints.Add(FVector(-300.f, 0.f, 90.f));
	MobSpawnPoints.Add(FVector(0.f, -300.f, 90.f));
	MobSpawnPoints.Add(FVector(300.f, 0.f, 90.f));
	MobSpawnPoints.Add(FVector(0.f, 300.f, 90.f));
	MobSpawnPoints.Add(FVector(-300.f, -300.f, 90.f));
	MobSpawnPoints.Add(FVector(300.f, 300.f, 90.f));
	MobSpawnPoints.Add(FVector(80.f, 80.f, 90.f));
}

void ARLBaseGameMode::BeginPlay()
{
	Super::BeginPlay();
}


void ARLBaseGameMode::RequestSpawnCell(int32 CellIndex, uint8 TempWall, uint8 Dir)
{
	DestroySpawnedActors();

	if (CellClasses.IsValidIndex(CellIndex))
	{
		SetSpawnedCell(GetWorld()->SpawnActor<ACellActor>(CellClasses[CellIndex], FVector::ZeroVector, FRotator::ZeroRotator));
		if (SpawnedCell)
		{
			SetPlayerLocation(Dir);
		}
	}
}

void ARLBaseGameMode::RequestSpawnHealItem()
{
	if (HealItemClass && GetWorld())
	{
		SpawnedHealItem = GetWorld()->SpawnActor<AHealItem>(HealItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void ARLBaseGameMode::CreateSidePortal() const
{
	if (SpawnedCell)
	{
		SpawnedCell->CreateSidePortal();
	}
}

void ARLBaseGameMode::CreateCenterPortal() const
{
	if (SpawnedCell)
	{
		SpawnedCell->CreateCenterPortal();
	}
}

void ARLBaseGameMode::DestroySpawnedActors()
{
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

void ARLBaseGameMode::SetPlayerLocation(uint8 Dir) const
{
	FVector PlayerLocation{ FVector::ZeroVector };
	if ((Dir != -1) && (static_cast<EDir>(Dir) != EDir::Load))
	{
		PlayerLocation = GetSpawnedCell()->GetPlayerSpawnLocation(Dir);
	}
	if (static_cast<EDir>(Dir) != EDir::Load)
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (PlayerCharacter)
		{
			const FVector PlayerLoc = PlayerCharacter->GetActorLocation();
			PlayerCharacter->SetActorLocation(FVector(PlayerLocation.X, PlayerLocation.Y, PlayerLoc.Z));
		}
	}
}

TSubclassOf<AMonsterCharacter> ARLBaseGameMode::GetNormalMonsterClass(EKindOfMonster KindOfMonster) const
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

TArray<UItemInfo*> ARLBaseGameMode::CreateRandItem()
{
	TArray<UItemInfo*> SelectedItem;

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

void ARLBaseGameMode::InitItemInfoFromTable()
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
				ItemInfos.Add(UItemInfo::ConstructItemInfo(ItemInfo->ItemsType, ItemInfo->DetailType, ItemInfo->ItemName, ItemInfo->ItemDesc, ItemInfo->ItemIcon, ItemInfo->HaveTooltip, ItemInfo->TooltipText));
			}
		}
	}
}
