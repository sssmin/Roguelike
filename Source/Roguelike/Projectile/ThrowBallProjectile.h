// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MonsterProjectile.h"
#include "ThrowBallProjectile.generated.h"


UCLASS()
class ROGUELIKE_API AThrowBallProjectile : public AMonsterProjectile
{
	GENERATED_BODY()
	
public:
	AThrowBallProjectile();
	virtual void BeginPlay() override;
	
protected:
	virtual void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
