// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MonsterProjectile.h"
#include "BombProjectile.generated.h"


UCLASS()
class ROGUELIKE_API ABombProjectile : public AMonsterProjectile
{
	GENERATED_BODY()
public:
	ABombProjectile();
	virtual void SetVelocity(const FVector& Dir) override;


protected:
	virtual void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
