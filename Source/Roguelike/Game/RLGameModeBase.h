// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "Roguelike/Type/DTForGM.h"
#include "RLGameModeBase.generated.h"

class AMonsterCharacter;
class ABossMonsterCharacter;
class AElementItem;
class AHealItem;
class ACellActor;

UCLASS()
class ROGUELIKE_API ARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARLGameModeBase();
	virtual void BeginPlay() override;
	void RequestSpawnMob(int32 StageLevel, int32 MobCount);
	void RequestSpawnBoss(int32 StageLevel);
	void RequestSpawnCell(int32 CellIndex, uint8 TempWall, int32 Dir = -1);
	void RequestSpawnHealItem();
	void CreateSidePortal();
	void CreateCenterPortal();
	void CreatePrevBossPortal();
	TArray<UItemInfo*> CreateRandItem();
	FVector GetBossCellScale();
private:
	UPROPERTY()
	TArray<UItemInfo*> SelectedItem;
	UPROPERTY()
	TArray<FVector> MobSpawnPoints;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ACellActor>> CellClasses;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AElementItem> ElementItemClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AHealItem> HealItemClass;
	UPROPERTY() 
	AHealItem* SpawnedHealItem;
	UPROPERTY()
	ACellActor* SpawnedCell;
	UPROPERTY()
	AElementItem* SpawnedElementItem;

	FCombatManager& SetRandomElement(int32 StageLevel, FCombatManager& CombatManager);
	void SpawnCounterElementItem(EElement Element);
	void SetMonsterManager(int32 StageLevel, OUT FHealthManager& HealthManager, OUT FCombatManager& CombatManager);
	void SetPlayerLocation(int32 Dir);
	TArray<AMonsterCharacter*> SpawnMob(int32 MobCount, EKindOfMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager);
	void SpawnBoss(EKindOfBossMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager);
	TSubclassOf<AMonsterCharacter> GetNormalMonsterClass(EKindOfMonster KindOfMonster);
	TSubclassOf<ABossMonsterCharacter> GetBossMonsterClass(EKindOfBossMonster KindOfMonster);
	void ConnectTurret(AMonsterCharacter* Turret, TArray<AMonsterCharacter*> SpawnedMobs);
};
