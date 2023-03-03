// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "MonsterProjectile.generated.h"

class UParticleSystem;

UCLASS()
class ROGUELIKE_API AMonsterProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	AMonsterProjectile();
	virtual void BeginPlay() override;
	
protected:
	virtual void Destroyed() override;
	
	
};
