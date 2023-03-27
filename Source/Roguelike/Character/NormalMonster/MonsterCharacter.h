// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/BaseCharacter.h"
#include "Roguelike/Interface/MonsterInterface.h"
#include "Roguelike/Component/MonsterCombatComponent.h"
#include "Roguelike/Type/DTForGM.h"
#include "MonsterCharacter.generated.h"

class USoundCue;
class UWidgetComponent;
class UHPBarWidget;
class UBossHPBarWidget;
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
	void SetHPBarWidget(const TSubclassOf<UBossHPBarWidget>& Widget);
	virtual void Attack() override;
	virtual void SpecialAttack(AActor* Target);
	virtual void GiveBTToController();
	virtual void OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType) override;
	void SetPatrolSpeed();
	void SetDefaultSpeed();
	void RequestHeal(AActor* Requester);
	virtual void Dead() override;
	virtual void SetIsDeadAnimInst();
	virtual void SetIsDeadBB();
	virtual void ShowNumWidget(float Damage, bool IsCritical, bool IsHeal, bool IsDodge) override;
	virtual void SetStateIcon(EState State) override;
	virtual void RemoveStateIcon(EState State) override;
	virtual void FlickerStateIcon(EState State) override;
	virtual void SetBuffIcon(EBuff Buff) override;
	virtual void RemoveBuffIcon(EBuff Buff) override;
	virtual void FlickerBuffIcon(EBuff Buff) override;
	
protected:
	void FireOneToTwo(TSubclassOf<UDamageType> DamageType);
	void ThrowBomb(AActor* Target, TSubclassOf<UDamageType> DamageType);
	void FireIn3Parts(TSubclassOf<UDamageType> DamageType);
	void FireIn8Parts(TSubclassOf<UDamageType> DamageType);
	void FireSpread8PartsFromCenter(TSubclassOf<UDamageType> DamageType);
	void Fire3Projectile(TSubclassOf<UDamageType> DamageType);
	void Meteor(const TSubclassOf<AMeteorActor>& Actor, AActor* Target);
	void RemoveHPWidget();
	UPROPERTY()
	ARLMonsterAIController* RLAIController;
	
private:
	void ExecuteDestroy();
	void CalcGiveBuffPer();
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UMonsterCombatComponent* MonsterCombatComp;
	
	EKindOfMonster KindOfMonster;
	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HPBarWidgetComp;
	UPROPERTY()
	UBossHPBarWidget* BossHPBarWidget;
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BT;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMeteorActor> MeteorActorClass;
	UPROPERTY()
	UHPBarWidget* HPBarWidget;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USoundCue* HealSoundCue;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HealParticle;
	
	float DefaultSpeed;
	float PatrolSpeed;
	UPROPERTY(VisibleAnywhere)
	EMonsterType MonsterType;
	
public:
	void SetMonsterType(EMonsterType Type) { MonsterType = Type; }
	EMonsterType GetMonsterType() const { return MonsterType; }
	UMonsterCombatComponent* GetMonsterCombatComp() const { return MonsterCombatComp; }
	TSubclassOf<AMeteorActor> GetMeteorActorClass() const { return MeteorActorClass; }
	void SetKindOfMonster(EKindOfMonster Kind) { KindOfMonster = Kind; }
	EKindOfMonster GetKindOfMonster() const { return KindOfMonster; }
	UBossHPBarWidget* GetBossHPBarWidget() const { return BossHPBarWidget; }
	
};
