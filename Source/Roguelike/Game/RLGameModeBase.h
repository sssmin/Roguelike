// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "Roguelike/Type/Manage.h"
#include "RLGameModeBase.generated.h"

USTRUCT(BlueprintType)
struct FMonsterStatTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvgATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvgMaxHP;

};

class AMonsterCharacter;
class AElementItem;
class AHealItem;

UCLASS()
class ROGUELIKE_API ARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARLGameModeBase();
	virtual void BeginPlay() override;
	void SpawnMob(int32 StageLevel, int32 MobCount);
	void SpawnBoss(int32 StageLevel);
	void SpawnHealItem();

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterCharacter> NormalMonsterClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterCharacter> EliteMonsterClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterCharacter> BossMonsterClass;
	TArray<FVector> MobSpawnPoints;
	void SpawnCounterElementItem(EElement Element);
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AElementItem> ElementItemClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AHealItem> HealItemClass;
	FCombatManage& SetRandomElement(int32 StageLevel, FCombatManage& CombatManage);

	void SetMonsterManage(int32 StageLevel, OUT FHealthManage& HealthManage, OUT FCombatManage& CombatManage);
};
