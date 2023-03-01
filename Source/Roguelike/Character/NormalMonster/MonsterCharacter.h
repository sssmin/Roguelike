// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/BaseCharacter.h"
#include "Roguelike/Interface/MonsterInterface.h"
#include "Roguelike/Component/MonsterCombatComponent.h"
#include "Roguelike/Type/DTForGM.h"
#include "MonsterCharacter.generated.h"

enum class EMonsterType : uint8
{
	NONE,
	NORMAL,
	ELITE,
	BOSS,

	MAX
};

class UWidgetComponent;
class UHPBarWidget;
class UMonsterCombatComponent;
class AMeteorActor;
class UBehaviorTree;
class ARLMonsterAIController;
class ATurretProjectile;

UCLASS()
class ROGUELIKE_API AMonsterCharacter : public ABaseCharacter, public IMonsterInterface
{
	GENERATED_BODY()

public:
	AMonsterCharacter();
	virtual void BeginPlay() override;
	void SetHPBarWidget(const TSubclassOf<UHPBarWidget>& Widget);
	virtual void Attack() override;
	virtual void SpecialAttack(AActor* Target);
	virtual void GiveBTToController();
	virtual void OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType) override;
	void SetPatrolSpeed();
	void SetDefaultSpeed();
	void RequestHeal(AActor* Requester);

protected:
	template<class T>
	void FireIn3Parts(TSubclassOf<UDamageType> DamageType, TSubclassOf<T> ProjectileClass)
	{
		if (MonsterCombatComp)
		{
			MonsterCombatComp->FireInParts<T>(3, 45.f, 45.f, DamageType, ProjectileClass);
		}
	}
	template<class T>
	void FireIn8Parts(TSubclassOf<UDamageType> DamageType, TSubclassOf<T> ProjectileClass)
	{
		if (MonsterCombatComp)
		{
			MonsterCombatComp->FireSpreadFromCenter<T>(8, 360.f, 45.f, DamageType, ProjectileClass);
		}
	}
	template<class T>
	void FireOneToTwo(TSubclassOf<UDamageType> DamageType, TSubclassOf<T> ProjectileClass)
	{
		if (MonsterCombatComp)
		{
			MonsterCombatComp->FireOneToTwo<T>(2, 15.f, 30.f, DamageType, ProjectileClass);
		}
	}
	template<class T>
	void FireSpread8PartsFromCenter(TSubclassOf<UDamageType> DamageType, TSubclassOf<T> ProjectileClass)
	{
		if (MonsterCombatComp)
		{
			MonsterCombatComp->FireSpreadFromCenter<T>(8, 360.f, 45.f, DamageType, ProjectileClass);
		}
	}
	template<class T>
	void ThrowBomb(TSubclassOf<T> ProjectileClass, AActor* Target, TSubclassOf<UDamageType> DamageType)
	{
		if (MonsterCombatComp)
		{
			MonsterCombatComp->ThrowBomb<T>(ProjectileClass, Target, DamageType);
		}
	}

	void Fire3Projectile(TSubclassOf<UDamageType> DamageType);
	void Teleport();
	void Meteor(const TSubclassOf<AMeteorActor>& Actor, AActor* Target);

	UPROPERTY()
	ARLMonsterAIController* RLAIController;
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UMonsterCombatComponent* MonsterCombatComp;

	EKindOfMonster KindOfMonster;

	EMonsterType MonsterType;

	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HPBarWidgetComp;
	

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BT;

	float DefaultSpeed;
	float PatrolSpeed;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOnetoAnotherProjectile> OnetoAnotherProjectileClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMeteorActor> MeteorActorClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABombProjectile> BombProjectileClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATurretProjectile> TurretProjectileClass;
public:
	void SetMonsterType(EMonsterType Type) { MonsterType = Type; }
	EMonsterType GetMonsterType() const { return MonsterType; }
	UMonsterCombatComponent* GetMonsterCombatComp() const { return MonsterCombatComp; }
	TSubclassOf<AOnetoAnotherProjectile> GetOnetoAnotherProjectileClass() const { return OnetoAnotherProjectileClass; }
	TSubclassOf<AMeteorActor> GetMeteorActorClass() const { return MeteorActorClass; }
	TSubclassOf<ABombProjectile> GetBombProjectileClass() const { return BombProjectileClass; }
	TSubclassOf<ATurretProjectile> GetTurretProjectileClass() const { return TurretProjectileClass;  }
	void SetKindOfMonster(EKindOfMonster Kind) { KindOfMonster = Kind; }
	EKindOfMonster GetKindOfMonster() const { return KindOfMonster; }
};
