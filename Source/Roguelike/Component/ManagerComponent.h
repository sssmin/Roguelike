// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "ManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateCurrentHP, float, CurrentHP, float, MaxHP);

class UItemComponent;
class UParticleSystem;
class AController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UManagerComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void Init();
	void ReceiveDamage(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType);
	void ApplyPlayerElement(EElement Element);
	void Heal(float Rate);
	bool CanAttack() const;
	bool CanMove() const;
	void UpdateMaxHP(float Value);
	void UpdateCurrentAtk(float Value);
	void UpdateCurrentCritical(float Value);
	void UpdateCurrentRange(float Value);
	bool HaveAnyState();
	bool IsDead();
	bool IsHPLow();
	void HPSync();
	FOnUpdateCurrentHP OnUpdateCurrentHP;

	static UManagerComponent* GetManagerComp(AActor* Character);

private:
	UFUNCTION()
	void SetTempManager() const;
	float CalcCounter(EElement EnemyElement);
	bool CheckState(uint8 State) const;
	void ApplyState(uint8 State);
	void RemoveState(uint8 State);
	bool CheckBuff(uint8 Buff) const;
	void ApplyBuff(uint8 Buff);
	void RemoveBuff(uint8 Buff);;
	void ApplyBurnDamage();
	void InitElemBuff();
	void ManageStack(float DeltaTime);
	void CalcCC(const FCombatManager& EnemyCombatManage);
	void ApplyCC();
	void CancelCC();
	bool IsDodge();
	float CalcCritical(const FCombatManager& EnemyCombatManage);
	void InitCCStack();
	void Dead();
	void UpdateCurrentHP(float Value);
	
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
	
	int32 CCStack;
	float CCStackDuration; //스택 지속시간
	float CCSeconds; //CC 경과시간
	float CCDuration; //CC 지속시간
	bool bShouldApplyCC;
	bool bIsBurn;
	float BurnTime;

public:
	void SetManager(const FHealthManager& InHealthManager, const FCombatManager& InCombatManager )
	{
		HealthManager = InHealthManager; 
		CombatManager = InCombatManager;
	}
	FCombatManager GetCombatManager() const { return CombatManager;  }
	void SetItemComp(UItemComponent* Comp) { ItemComponent = Comp; }
	uint8 GetCurrentState() const { return CurrentState; }
	float GetMaxHP() const { return HealthManager.MaxHP; }
};
