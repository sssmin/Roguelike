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
	int32 StageLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AvgATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AvgMaxHP;

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
UCLASS()
class ROGUELIKE_API ARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARLGameModeBase();
	virtual void BeginPlay() override;
	void SpawnMob(int32 StageLevel, int32 MobCount);
	void SpawnBoss(int32 StageLevel);
	
};
