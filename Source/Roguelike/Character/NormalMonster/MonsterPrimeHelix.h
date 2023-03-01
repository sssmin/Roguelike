// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "MonsterPrimeHelix.generated.h"

class AMeteorActor;

UCLASS()
class ROGUELIKE_API AMonsterPrimeHelix : public AMonsterCharacter
{
	GENERATED_BODY()
public:

protected:
	virtual void SpecialAttack(AActor* Target) override;


	
};
