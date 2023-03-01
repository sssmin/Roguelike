// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "MonsterDino.generated.h"


UCLASS()
class ROGUELIKE_API AMonsterDino : public AMonsterCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void SpecialAttack(AActor* Target) override;
};
