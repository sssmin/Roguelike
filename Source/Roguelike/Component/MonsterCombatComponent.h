// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatComponent.h"
#include "MonsterCombatComponent.generated.h"

class AMonsterProjectile;

UCLASS()
class ROGUELIKE_API UMonsterCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
public:
	UMonsterCombatComponent();
	virtual void ReadyToFire(bool bPressed) override;
	void FireInParts(int32 Parts, float StartDegree, float DeltaDegree);
	void FireOneToTwo(int32 Parts, float StartDegree, float DeltaDegree);
private:
	void Fire(const FCombatManager& CombatManager);

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterProjectile> ProjectileClass;
};
