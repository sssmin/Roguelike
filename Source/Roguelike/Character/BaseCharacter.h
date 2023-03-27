// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

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
	virtual void OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType);
	virtual void Attack();
	virtual void Dead();
	bool IsDead();
	FCombatManager GetCombatManager() const;
	virtual void ShowNumWidget(float Damage, bool IsCritical, bool IsHeal, bool IsDodge);
	virtual void SetStateIcon(EState State);
	virtual void RemoveStateIcon(EState State);
	virtual void FlickerStateIcon(EState State);
	virtual void SetBuffIcon(EBuff Buff);
	virtual void RemoveBuffIcon(EBuff Buff);
	virtual void FlickerBuffIcon(EBuff Buff);
	virtual void HealByHit(float Rate) const;
	
protected:
	virtual void Destroyed() override;
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UManagerComponent* ManagerComponent;
	
private:
	UFUNCTION()
	void OnExplodeHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY()
	TSubclassOf<UObject> DamageWidgetActorClass;
	
	
public:
	UFUNCTION(BlueprintCallable)
	UManagerComponent* GetManagerComp() const { return ManagerComponent;  }
	TSubclassOf<UObject> GetDamageWidgetActorClass() const { return DamageWidgetActorClass; }
	
};
