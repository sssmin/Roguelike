// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "RLGameModeBase.generated.h"

class UTexture2D;
class AMonsterCharacter;
class AElementItem;
class AHealItem;
class ACellActor;


USTRUCT(BlueprintType)
struct FMonsterStatTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvgATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvgMaxHP;

};





UCLASS()
class ROGUELIKE_API ARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARLGameModeBase();
	virtual void BeginPlay() override;
	void SpawnMob(int32 StageLevel, int32 MobCount);
	void SpawnBoss(int32 StageLevel);
	void SpawnCell(int32 CellIndex, uint8 TempWall, int32 Dir = -1);
	void SpawnHealItem();
	void CreateSidePortal();
	void CreateCenterPortal();
	void CreatePrevBossPortal();
	TArray<UItemInfo*> CreateRandItem();
private:
	UPROPERTY()
	TArray<UItemInfo*> SelectedItem;
	UPROPERTY()
	TArray<FVector> MobSpawnPoints;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AMonsterCharacter>> NormalMonsterClasses;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterCharacter> BossMonsterClass;
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
};
