// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "BossMonsterCharacter.generated.h"


UCLASS()
class ROGUELIKE_API ABossMonsterCharacter : public AMonsterCharacter
{
	GENERATED_BODY()
public:

private:
	void RandSpawnRandAdvance();
};
