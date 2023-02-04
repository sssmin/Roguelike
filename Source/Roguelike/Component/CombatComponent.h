// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Manage.h"
#include "CombatComponent.generated.h"


DECLARE_DELEGATE_RetVal(FCombatManage, FGetCombatManage);

class ABaseProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Fire();
	FGetCombatManage GetCombatManage;


private:
	TSubclassOf<ABaseProjectile> ProjectileClass;

public:	
	void SetProjectileClass(TSubclassOf<ABaseProjectile> Class) { ProjectileClass = Class;  }
	
		
};
