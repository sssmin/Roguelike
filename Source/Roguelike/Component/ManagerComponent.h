// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Type/Manage.h"
#include "ManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UManagerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReceiveDamage(const FCombatManage& EnemyCombatManage, const FItemManage& EnemyItemManage);
	void ApplyPlayerElement(int32 ConvertElement);
	void Heal(float Rate);
	bool CanAttack();
	bool CanMove();

	void TestDead();
	void TestHurt();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	FHealthManage HealthManager;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	FCombatManage CombatManager;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	FItemManage ItemManager;

	UPROPERTY(Meta = (Bitmask, BitmaskEnum = EState))
	uint8 CurrentState;

	UPROPERTY(Meta = (Bitmask, BitmaskEnum = EBuff))
	uint8 CurrentBuff;

	void SendManager();
	float CalcCounter(EElement EnemyElement);
	bool CheckState(uint8 State);
	void ApplyState(uint8 State);
	void RemoveState(uint8 State);
	bool CheckBuff(uint8 Buff);
	void ApplyBuff(uint8 Buff);
	void RemoveBuff(uint8 Buff);
	bool CheckItem(uint8 Item);
	void ApplyItem(uint8 Item);
	void RemoveItem(uint8 Item);

	bool HaveAnyState();
	void ApplyBurnDamage();
	void ApplyItem();
	void CovertElementFromInt(int32 ConvertElement);
	void InitElemBuff();
	void ManageStack(float DeltaTime);
	void CalcCC(const FCombatManage& EnemyCombatManage);
	void ApplyCC();
	void CancelCC();
	

	int32 CCStack;
	float CCStackDuration; //스택 지속시간
	float CCSeconds; //CC 경과시간
	float CCDuration; //CC 지속시간
	bool bShouldApplyCC;
	bool bIsBurn;
	float BurnTime;

	void Dead();
public:
	void SetManager(const FHealthManage& InHealthManager, const FCombatManage& InCombatManager )
	{
		HealthManager = InHealthManager; 
		CombatManager = InCombatManager;
	}
	FCombatManage GetCombatManage() const { return CombatManager;  }
	FItemManage GetItemManage() const { return ItemManager;  }
	
};
