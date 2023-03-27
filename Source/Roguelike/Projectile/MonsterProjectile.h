// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "MonsterProjectile.generated.h"

class USoundCue;
class UParticleSystem;

UCLASS()
class ROGUELIKE_API AMonsterProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	AMonsterProjectile();
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	

private:
	UPROPERTY() 
	USoundCue* SpawnSoundCue;
	
};
