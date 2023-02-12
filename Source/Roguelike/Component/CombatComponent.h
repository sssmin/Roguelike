// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "CombatComponent.generated.h"


DECLARE_DELEGATE_RetVal(FCombatManager, FGetCombatManager);
DECLARE_DELEGATE_RetVal(FItemManager, FGetItemManager);

class ABaseProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReadyToFire(bool bPressed);
	FGetCombatManager GetCombatManager;
	FGetItemManager GetItemManager;

	bool HaveItem(const FItemManager& Manager, EOnceEquippedItem ItemType);
private:
	TSubclassOf<ABaseProjectile> ProjectileClass;
	UFUNCTION()
	void Fire(const FCombatManager& CombatManager, const FItemManager& ItemManager);
	void Fire(const FCombatManager& CombatManager);
	FTimerHandle MultiShotTimerHandle;
	float MutliShotTime;

	bool bFireCooldown;
	bool bAttackPressed;
	float Delay;

	void StartFireTimer();
	FTimerHandle FireTimerHandle;
	void FireTimerFinished();
public:	
	void SetProjectileClass(TSubclassOf<ABaseProjectile> Class) { ProjectileClass = Class;  }
	
		
};
