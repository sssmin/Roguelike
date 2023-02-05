// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Manage.h"
#include "ManagerComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UManagerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReceiveDamage(const FCombatManage& EnemyCombatManage);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	FHealthManage HealthManager;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	FCombatManage CombatManager;

	UPROPERTY(Meta = (Bitmask, BitmaskEnum = EStat))
	uint8 CurrentState;

	void SendManager();
	float CalcCounter(EElement EnemyElement);
	bool CheckState(EState State);
	void GiveState(EState State);
	void RemoveState(EState State);

	void Dead();
public:
	void SetManager(const FHealthManage& InHealthManager, const FCombatManage& InCombatManager )
	{
		HealthManager = InHealthManager; 
		CombatManager = InCombatManager;
	}
	FCombatManage GetCombatManage() const { return CombatManager;  }
};
