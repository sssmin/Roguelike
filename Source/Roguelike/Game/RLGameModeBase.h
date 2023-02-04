// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
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

//USTRUCT(BlueprintType)
//struct FMonsterTypeTable : public FTableRowBase
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	EMonsterType Type;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int32 AvgATK;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int32 AvgMaxHP;
//};

enum class EMonsterType : uint8
{
	NONE,
	NORMAL,
	ELITE,
	BOSS,

	MAX
};

class AMonsterCharacter;
struct FHealthManage;
struct FCombatManage;

UCLASS()
class ROGUELIKE_API ARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARLGameModeBase();
	virtual void BeginPlay() override;
	void SpawnMob(int32 StageLevel, int32 MobCount);
	void SpawnBoss(int32 StageLevel);

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterCharacter> MonsterClass;
	TArray<FVector> MobSpawnPoints;

	void SetMonsterManage(int32 StageLevel, OUT FHealthManage& HealthManage, OUT FCombatManage& CombatManage);
};
