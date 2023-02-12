// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Roguelike/Roguelike.h"
#include "GameFramework/Character.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "BaseCharacter.generated.h"



class UCombatComponent;
class ABaseProjectile;
class UManagerComponent;

UCLASS()
class ROGUELIKE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnHit(AActor* Attacker, const FCombatManager& EnemyManager, const FItemManager& EnemyItemManager);
	void Dead();
protected:
	virtual void Attack();
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UManagerComponent* ManagerComponent;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;
private:
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseProjectile> ProjectileClass;
	

	FCombatManager GetCombatManager() const;
	
public:
	UFUNCTION(BlueprintCallable)
	UManagerComponent* GetManagerComp() const { return ManagerComponent;  }
};
