// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DTForGM.generated.h"

class AMonsterCharacter;
class ABossMonsterCharacter;

enum class EDir : uint8
{
	Up,
	Down,
	Right,
	Left ,
	Load 
};

enum class EMonsterType : uint8
{
	None,
	Normal,
	Elite,
	Boss,
	Ego,

	MAX
};

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
	Turret = 0,
	Dino,
	Bee,
	White,
	PrimeHelix,
	Skeleton,
	Suicide,
	Wizard,

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
	Usurper = 0,
	SoulEater,
	TerrorBringer,

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
