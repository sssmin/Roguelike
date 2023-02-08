// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Type/Manage.h"
#include "CombatComponent.generated.h"


DECLARE_DELEGATE_RetVal(FCombatManage, FGetCombatManage);
DECLARE_DELEGATE_RetVal(FItemManage, FGetItemManage);

class ABaseProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReadyToFire();
	FGetCombatManage GetCombatManage;
	FGetItemManage GetItemManage;

	bool HaveItem(const FItemManage& Manage, EOnceEquippedItem ItemType);
private:
	TSubclassOf<ABaseProjectile> ProjectileClass;
	UFUNCTION()
	void Fire(const FCombatManage& CombatManage, const FItemManage& ItemManage);
	FTimerHandle MultiShotTimerHandle;
	float MutliShotTime;
public:	
	void SetProjectileClass(TSubclassOf<ABaseProjectile> Class) { ProjectileClass = Class;  }
	
		
};
