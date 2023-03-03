// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterProjectile.h"
#include "OnetoAnotherProjectile.generated.h"


UCLASS()
class ROGUELIKE_API AOnetoAnotherProjectile : public AMonsterProjectile
{
	GENERATED_BODY()
	
public:
	AOnetoAnotherProjectile();
	void OneToTwo(const FVector& Dir);
	
protected:
	virtual void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	UFUNCTION()
	void ChangeDir(const FVector& Dir);
	
};
