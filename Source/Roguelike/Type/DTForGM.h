// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "DTForGM.generated.h"


class AMonsterCharacter;
class ABossMonsterCharacter;

USTRUCT(BlueprintType)
struct FMonsterStatTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvgATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AvgMaxHP;
};

UENUM()
enum class EKindOfMonster : uint8
{
	TURRET = 0,
	DINO,
	BEE,
	WHITE,
	PRIMEHELIX,
	SKELETON,
	SUICIDE,
	WIZARD,

	MAX
};

USTRUCT(BlueprintType)
struct FNormalMonsterClassTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMonsterCharacter> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKindOfMonster KindOfMonster;

};

UENUM()
enum class EKindOfBossMonster : uint8
{
	USURPER = 0,
	SOULEATER,
	TERRORBRINGER,

	MAX
};

USTRUCT(BlueprintType)
struct FBossMonsterClassTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABossMonsterCharacter> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKindOfBossMonster KindOfMonster;

};


UCLASS()
class ROGUELIKE_API UDTForGM : public UObject
{
	GENERATED_BODY()
	
};
