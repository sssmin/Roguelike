// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "MonsterBossEgo.generated.h"


UCLASS()
class ROGUELIKE_API AMonsterBossEgo : public AMonsterCharacter
{
	GENERATED_BODY()
public:
	AMonsterBossEgo();
	virtual void GiveBTToController() override;
	void SetBoss(AActor* InBoss);
	virtual void SpecialAttack(AActor* Target) override;
	
protected:
	virtual void OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType) override;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	AActor* Boss;
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* EgoBT;
	
};
