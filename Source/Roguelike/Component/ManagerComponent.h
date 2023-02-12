// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "ManagerComponent.generated.h"

class UItemComponent;
class UParticleSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UManagerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReceiveDamage(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager);
	void ApplyPlayerElement(EElement Element);
	void Heal(float Rate);
	bool CanAttack();
	bool CanMove();
	void UpdateMaxHP(float Value);
	void UpdateCurrentHP(float Value);
	void UpdateCurrentAtk(float Value);
	void UpdateCurrentCritical(float Value);
	void UpdateCurrentRange(float Value);

	void TestDead();
	void TestHurt();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	FHealthManager HealthManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	FCombatManager CombatManager;

	UPROPERTY(Meta = (Bitmask, BitmaskEnum = EState))
	uint8 CurrentState;

	UPROPERTY(Meta = (Bitmask, BitmaskEnum = EBuff))
	uint8 CurrentBuff;

	UPROPERTY()
	UItemComponent* ItemComponent;



	void SendManager();
	float CalcCounter(EElement EnemyElement);
	bool CheckState(uint8 State);
	void ApplyState(uint8 State);
	void RemoveState(uint8 State);
	bool CheckBuff(uint8 Buff);
	void ApplyBuff(uint8 Buff);
	void RemoveBuff(uint8 Buff);;
	
	bool HaveAnyState();
	void ApplyBurnDamage();
	void InitElemBuff();
	void ManageStack(float DeltaTime);
	void CalcCC(const FCombatManager& EnemyCombatManage);
	void ApplyCC();
	void CancelCC();
	bool IsDodge();
	float CalcCritical(const FCombatManager& EnemyCombatManage);

	int32 CCStack;
	float CCStackDuration; //스택 지속시간
	float CCSeconds; //CC 경과시간
	float CCDuration; //CC 지속시간
	bool bShouldApplyCC;
	bool bIsBurn;
	float BurnTime;

	void Dead();
public:
	void SetManager(const FHealthManager& InHealthManager, const FCombatManager& InCombatManager )
	{
		HealthManager = InHealthManager; 
		CombatManager = InCombatManager;
	}
	FCombatManager GetCombatManager() const { return CombatManager;  }
	void SetItemComp(UItemComponent* Comp) { ItemComponent = Comp; }
	
};
