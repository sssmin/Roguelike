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
class AMonsterBossEgo;
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
	void RequestSpawnCell(int32 CellIndex, uint8 TempWall, uint8 Dir = -1);
	void RequestSpawnHealItem();
	void CreateSidePortal() const;
	void CreateCenterPortal() const;
	void CreatePrevBossPortal() const;
	TArray<UItemInfo*> CreateRandItem();
	TArray<UItemInfo*> CreateLoadItem();
	FVector GetBossCellScale() const;
	void SetItemIcon(TArray<UItemInfo*>& InItemInfos);
	
private:
	void InitItemInfoFromTable();
	FCombatManager& SetRandomElement(int32 StageLevel, FCombatManager& CombatManager);
	void SpawnCounterElementItem(EElement Element);
	void SetMonsterManager(int32 StageLevel, OUT FHealthManager& HealthManager, OUT FCombatManager& CombatManager) const;
	void SetPlayerLocation(uint8 Dir) const;
	TArray<AMonsterCharacter*> SpawnMob(int32 MobCount, EKindOfMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager);
	void SpawnBoss(EKindOfBossMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager);
	TSubclassOf<AMonsterCharacter> GetNormalMonsterClass(EKindOfMonster KindOfMonster) const;
	TSubclassOf<ABossMonsterCharacter> GetBossMonsterClass(EKindOfBossMonster KindOfMonster) const;
	void ConnectTurret(AMonsterCharacter* Turret, TArray<AMonsterCharacter*> SpawnedMobs);
	AMonsterCharacter* SpawnBossEgo(const FCombatManager& CombatManager, AMonsterCharacter* Boss);
	void DestroySpawnedActors();
	
	UPROPERTY()
	TArray<UItemInfo*> ItemInfos;
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
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterBossEgo> BossEgoClass;

	
};
