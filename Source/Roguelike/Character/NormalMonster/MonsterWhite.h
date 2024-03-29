// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "MonsterWhite.generated.h"


class USoundCue;

UCLASS()
class ROGUELIKE_API AMonsterWhite : public AMonsterCharacter
{
	GENERATED_BODY()

public:
	virtual void SpecialAttack(AActor* Target) override;
	virtual void Dead() override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USoundCue* DestroySoundCue;
	
};
