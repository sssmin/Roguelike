// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "RLBaseGameMode.h"
#include "Roguelike/Type/StatManage.h"
#include "RLMainGameMode.generated.h"

class AMonsterCharacter;
class ABossMonsterCharacter;
class AMonsterBossEgo;
class AElementItem;
class AHealItem;
class ACellActor;

UCLASS()
class ROGUELIKE_API ARLMainGameMode  : public ARLBaseGameMode
{
	GENERATED_BODY()
	
public:
	ARLMainGameMode();
	virtual void BeginPlay() override;
	void RequestSpawnMob(int32 StageLevel, int32 MobCount);
	void RequestSpawnBoss(int32 StageLevel);
	virtual void RequestSpawnCell(int32 CellIndex, uint8 TempWall, uint8 Dir = -1) override;
	virtual void RequestSpawnHealItem() override;
	virtual void CreateSidePortal() const override;
	virtual void CreateCenterPortal() const override;
	virtual void DestroySpawnedActors() override;
	void CreatePrevBossPortal() const;
	
	TArray<UItemInfo*> CreateLoadItem();
	FVector GetBossCellScale() const;
	void SetItemIcon(TArray<UItemInfo*>& InItemInfos);
	

private:
	FCombatManager& SetRandomElement(int32 StageLevel, FCombatManager& CombatManager);
	void SpawnCounterElementItem(EElement Element);
	void SetMonsterManager(int32 StageLevel, OUT FHealthManager& HealthManager, OUT FCombatManager& CombatManager) const;
	void SetPlayerLocation(uint8 Dir) const;
	TArray<AMonsterCharacter*> SpawnMob(int32 MobCount, EKindOfMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager);
	void SpawnBoss(EKindOfBossMonster KindOfMonster, FCombatManager& CombatManager, FHealthManager& HealthManager);
	TSubclassOf<ABossMonsterCharacter> GetBossMonsterClass(EKindOfBossMonster KindOfMonster) const;
	void ConnectTurret(AMonsterCharacter* Turret, TArray<AMonsterCharacter*> SpawnedMobs);
	AMonsterCharacter* SpawnBossEgo(const FCombatManager& CombatManager, AMonsterCharacter* Boss);
	void InitImage();

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AElementItem> ElementItemClass;
	UPROPERTY()
	AElementItem* SpawnedElementItem;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterBossEgo> BossEgoClass;
	UPROPERTY()
	TMap<EState, UTexture2D*> StateImages;
	UPROPERTY()
	TMap<EBuff, UTexture2D*> BuffImages;
	UPROPERTY()
	TMap<EINFStackItem, UTexture2D*> StatImages;

public:
	UTexture2D* GetStateImage(EState State);
	UTexture2D* GetBuffImage(EBuff Buff);
	UTexture2D* GetStatImage(EINFStackItem StatItem);
	
	
};
