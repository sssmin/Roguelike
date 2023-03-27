// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "TutorialMonster.generated.h"

UCLASS()
class ROGUELIKE_API ATutorialMonster : public AMonsterCharacter
{
	GENERATED_BODY()

public:
	virtual void OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType) override;
	
};
