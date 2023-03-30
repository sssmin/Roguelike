// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BossMonsterCharacter.h"
#include "BossSoulEater.generated.h"


UCLASS()
class ROGUELIKE_API ABossSoulEater : public ABossMonsterCharacter
{
	GENERATED_BODY()

public:
	ABossSoulEater();
	virtual void ExecuteSkill() override;
	virtual void BeginPlay() override;
	
};
