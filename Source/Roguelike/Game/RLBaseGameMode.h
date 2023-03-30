// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Roguelike/Type/DTForGM.h"
#include "Roguelike/Type/ItemManage.h"
#include "RLBaseGameMode.generated.h"

class ACellActor;
class AHealItem;
class AMonsterBossEgo;


UCLASS()
class ROGUELIKE_API ARLBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARLBaseGameMode();
	virtual void BeginPlay() override;
	virtual void RequestSpawnCell(int32 CellIndex, uint8 TempWall, uint8 Dir = -1);
	virtual void RequestSpawnHealItem();
	virtual void CreateSidePortal() const;
	virtual void CreateCenterPortal() const;
	virtual void DestroySpawnedActors();
	virtual void SetPlayerLocation(uint8 Dir) const;
	TArray<UItemInfo*> CreateRandItem();
	void InitItemInfoFromTable();
	virtual TSubclassOf<AMonsterCharacter> GetNormalMonsterClass(EKindOfMonster KindOfMonster) const;

private:
	UPROPERTY()
	TArray<UItemInfo*> ItemInfos;
	UPROPERTY()
	TArray<FVector> MobSpawnPoints;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ACellActor>> CellClasses;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AHealItem> HealItemClass;
	UPROPERTY() 
	AHealItem* SpawnedHealItem;
	UPROPERTY()
	ACellActor* SpawnedCell;
	

public:
	TArray<FVector> GetMobSpawnPoints() const { return MobSpawnPoints; }
	TArray<TSubclassOf<ACellActor>> GetCellClasses() const { return CellClasses; }
	TSubclassOf<AHealItem> GetHealItemClass() const { return HealItemClass; }
	AHealItem* GetSpawnedHealItem() const { return SpawnedHealItem; }
	void SetSpawnedHealItem(AHealItem* InHealItem) { SpawnedHealItem = InHealItem; }
	ACellActor* GetSpawnedCell() const { return SpawnedCell; }
	void SetSpawnedCell(ACellActor* InCellActor) { SpawnedCell = InCellActor; }
	TArray<UItemInfo*> GetItemInfos() const { return ItemInfos; }
	
};
