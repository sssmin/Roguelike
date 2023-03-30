// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Roguelike/Type/DTForGM.h"
#include "MonsterObjectPool.generated.h"


class AMonsterCharacter;
UCLASS()
class ROGUELIKE_API UMonsterObjectPool : public UObject
{
	GENERATED_BODY()

public:
	void Init();
	AMonsterCharacter* GetNormalMonster(EKindOfMonster KindOfMonster);
	AMonsterCharacter* GetDeactivateMonster(TArray<AMonsterCharacter*> Pool);
	AMonsterCharacter* GetBossMonster(EKindOfBossMonster KindOfMonster);
	AMonsterCharacter* GetBossEgoMonster() const { return BossEgoPool; }
	
private:
	void NormalMonsterInit();
	void BossMonsterInit();
	void BossEgoInit();
	
	int32 PoolSize = 6;
	UPROPERTY()
	TArray<AMonsterCharacter*> BeePool;
	UPROPERTY()
	TArray<AMonsterCharacter*> DinoPool;
	UPROPERTY()
	TArray<AMonsterCharacter*> PrimeHelixPool;
	UPROPERTY()
	TArray<AMonsterCharacter*> SkeletonPool;
	UPROPERTY()
	TArray<AMonsterCharacter*> SuicidePool;
	UPROPERTY()
	TArray<AMonsterCharacter*> WhitePool;
	UPROPERTY()
	TArray<AMonsterCharacter*> WizardPool;

	UPROPERTY()
	AMonsterCharacter* TurretPool;
	UPROPERTY()
	ABossMonsterCharacter* UsurperPool;
	UPROPERTY()
	ABossMonsterCharacter* SoulEaterPool;
	UPROPERTY()
	ABossMonsterCharacter* TerrorBringerPool;
	UPROPERTY()
	AMonsterCharacter* BossEgoPool;
	
};

