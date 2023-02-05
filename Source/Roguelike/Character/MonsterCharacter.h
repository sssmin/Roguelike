// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MonsterCharacter.generated.h"

enum class EMonsterType : uint8
{
	NONE,
	NORMAL,
	ELITE,
	BOSS,

	MAX
};

UCLASS()
class ROGUELIKE_API AMonsterCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AMonsterCharacter();
	
private:
	EMonsterType MonsterType;
	
public:
	void SetMonsterType(EMonsterType Type) { MonsterType = Type; }
};
