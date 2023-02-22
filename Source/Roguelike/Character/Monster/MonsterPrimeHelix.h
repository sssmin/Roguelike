// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/Monster/MonsterCharacter.h"
#include "MonsterPrimeHelix.generated.h"

class UNiagaraSystem;
class AMeteorActor;

UCLASS()
class ROGUELIKE_API AMonsterPrimeHelix : public AMonsterCharacter
{
	GENERATED_BODY()
public:

protected:
	virtual void SpecialAttack(AActor* Target) override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMeteorActor> MeteorActor;
	
};
