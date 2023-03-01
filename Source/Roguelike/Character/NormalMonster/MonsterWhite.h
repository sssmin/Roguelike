// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "MonsterWhite.generated.h"


UCLASS()
class ROGUELIKE_API AMonsterWhite : public AMonsterCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void SpecialAttack(AActor* Target) override;
	virtual void Destroyed() override;
	
};
