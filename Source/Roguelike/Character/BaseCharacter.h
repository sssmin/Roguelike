// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Roguelike/Roguelike.h"
#include "GameFramework/Character.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "BaseCharacter.generated.h"


class UManagerComponent;
class UDamageType;
class AController;

UCLASS()
class ROGUELIKE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnHit(AActor* Attacker, const FCombatManager& EnemyManager, const FItemManager& EnemyItemManager);
	virtual void Attack();
	void Dead();

protected:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UManagerComponent* ManagerComponent;
	

private:
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);

public:
	FCombatManager GetCombatManager() const;
	UFUNCTION(BlueprintCallable)
	UManagerComponent* GetManagerComp() const { return ManagerComponent;  }

};
