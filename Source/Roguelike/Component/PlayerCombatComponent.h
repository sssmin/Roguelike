// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "CombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class ABaseProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashCooldownComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSetDashCooldown);

UCLASS()
class ROGUELIKE_API UPlayerCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
	
public:
	UPlayerCombatComponent();
	virtual void ReadyToFire(bool bPressed) override;
	bool CanDash() const;
	void SetDashCooldown();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDashCurrentCooltime() const;
	UFUNCTION()
	void DashCooldownComplete();
	UPROPERTY(BlueprintAssignable)
	FOnDashCooldownComplete OnDashCooldownCompleteDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnSetDashCooldown OnSetDashCooldownDelegate;
	
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
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseProjectile> ProjectileClass;
	
	bool bAttackPressed;
	bool bFireCooldown;
	float Delay;
	float MutliShotTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentDashChargeNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	int32 MaxDashChargeNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float DashChargeCooltime;
	FTimerHandle DashChargeTimerHandle;

public:
	
	
};
