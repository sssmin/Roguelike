// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "ManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateCurrentHP, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetStateNum, int32, Value);

class UItemComponent;
class UParticleSystem;
class AController;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UManagerComponent();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void Load();
	UFUNCTION()
	void Init();
	void ReceiveDamage(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType);
	void ApplyPlayerElement(EElement Element);
	void HealByRate(float Rate);
	void HealByValue(float Value);
	bool CanAttack() const;
	bool CanMove() const;
	void UpdateMaxHP(float Value);
	void UpdateCurrentAtk(float Value);
	void UpdateCurrentCritical(float Value);
	void UpdateCurrentRange(float Value);
	UFUNCTION()
	bool HaveAnyState();
	bool IsDead();
	bool IsHPLow();
	void HPSync();
	FOnUpdateCurrentHP OnUpdateCurrentHP;
	FOnSetStateNum OnSetStateNum;
	void ApplyMovementBuff();

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
	UFUNCTION()
	void RemoveBuff(uint8 Buff);
	void ApplyDoTDamage(float Rate);
	void InitElemBuff();
	void CalcStateStack(const FCombatManager& EnemyCombatManage);
	void InitState();
	bool IsDodge();
	float CalcCritical(const FCombatManager& EnemyCombatManage);
	void InitStateStack(uint8 State);
	void Dead();
	void UpdateCurrentHP(float Value);
	void SetStateTimer(float StartDuration, float RestDuration, float StateSecond, float DoTDamageRate, FTimerHandle& StartHandle, FTimerHandle& RestHandle, FTimerHandle& DamageHandle, uint8 State, ABaseCharacter* OwnerCharacter);
	void StateCompleted(uint8 CompleteState);
	void SetBuffTimer(float StartDuration, float RestDuration,  FTimerHandle& StartHandle, FTimerHandle& RestHandle,  uint8 Buff, ABaseCharacter* OwnerCharacter);
	void BuffCompleted(uint8 CompleteBuff);
	UFUNCTION(BlueprintCallable)
	void RemoveAllTimer();
	void RemoveTimer(FTimerHandle Handle);
	
	
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
	int32 StateNum;
	
	int32 BurnStack;
	int32 FrozenStack;
	int32 PoisonStack;
	int32 FearStack;
	float StateStackDuration; //���� ���ӽð�
	float StateSeconds; //�����̻� ����ð�
	float StateStartDuration; //�����̻� �������� �� ���� �ð� 2��
	float StateRestDuration; //�����̻� �������ð� 3��
	float BuffDuration;
	float DoTDamageRate;

	FTimerHandle BurnStackDurationTimerHandle; //ȭ���� ���ӽð�
	FTimerHandle FrozenStackDurationTimerHandle; //������ ���ӽð�
	FTimerHandle PoisonStackDurationTimerHandle; //�ߵ����� ���ӽð�
	FTimerHandle FearStackDurationTimerHandle; //�������� ���ӽð�
	
	FTimerHandle BurnStartTimerHandle; //ȭ�� ���ӽð�
	FTimerHandle BurnRestTimerHandle; //ȭ�� ���ӽð�
	FTimerHandle BurnApplyDamageTimerHandle; //ȭ�� ����� ������ �ð� ����
	
	FTimerHandle FrozenStartTimerHandle; //���� ���ӽð�
	FTimerHandle FrozenRestTimerHandle; //���� ���ӽð�
	FTimerHandle FrozenApplyDamageTimerHandle; //���� ����� ������ �ð� ����
	
	FTimerHandle PoisonStartTimerHandle; //�ߵ� ���ӽð�
	FTimerHandle PoisonRestTimerHandle; //�ߵ� ���ӽð�
	FTimerHandle PoisonApplyDamageTimerHandle; //�ߵ� ����� ������ �ð� ����
	
	FTimerHandle FearDurationTimerHandle; //���� ���ӽð�
	FTimerHandle FearApplyDamageTimerHandle; //���� ����� ������ �ð� ����(�÷��̾�)
	
	FTimerHandle MoveBuffStartTimerHandle; //�̼� ���� ���ӽð�
	FTimerHandle MoveBuffRestTimerHandle; //�̼� ���� ���ӽð�

	TArray<FTimerHandle> TimerHandles;

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
