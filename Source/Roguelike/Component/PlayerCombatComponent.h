// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class ABaseProjectile;

UCLASS()
class ROGUELIKE_API UPlayerCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
public:
	UPlayerCombatComponent();
	virtual void ReadyToFire(bool bPressed) override;

private:
	void Fire(const FCombatManager& CombatManager, const FItemManager& ItemManager);
	UFUNCTION()
	void Multishot(const FCombatManager& CombatManager, const FItemManager& ItemManager);
	UFUNCTION()
	void Triple(const FCombatManager& CombatManager, const FItemManager& ItemManager);
	bool NormalFire(const FCombatManager& CombatManager, const FItemManager& ItemManager);
	bool TripleFire(const FCombatManager& CombatManager, const FItemManager& ItemManager);
	void StartFireTimer();
	void FireTimerFinished();
	bool HaveItem(const FItemManager& Manager, EOnceEquippedItem ItemType);
	

	bool bAttackPressed;
	bool bFireCooldown;
	float Delay;
	float MutliShotTime;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseProjectile> ProjectileClass;

	
};
