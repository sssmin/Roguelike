// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "BossMonsterCharacter.generated.h"

class UBossCombatComponent;
class AMeteorActor;
class AWhirlwindActor;
class ABreathActor;
class UAnimMontage;
class AThrowBallProjectile;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class ROGUELIKE_API ABossMonsterCharacter : public AMonsterCharacter
{
	GENERATED_BODY()
public:
	ABossMonsterCharacter();
	virtual void BeginPlay() override;
	virtual void ExecuteSkill();
	virtual void GiveBTToController() override;
	void OnSkillEnd();
	void OnExecuteBreath();
	void OnExecuteThrowBall();
	void OnExecuteWhirlwind();
	void OnExecuteSquare();
	virtual void SetIsDeadAnimInst() override;
	virtual void SetIsDeadBB() override;
	virtual void Dead() override;
	void RemoveSpawnedWhirlwindActor(AWhirlwindActor* Actor);
	
protected:
	virtual void OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType) override;
	virtual void Whirlwind();
	virtual void ThrowBall();
	virtual void Breath();
	virtual void Square();
	virtual void Destroyed() override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BreathMontage;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ThrowMontage;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* WhirlwindMontage;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SquareMontage;
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BossBT;
	UPROPERTY()
	AMonsterCharacter* BossEgo;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBossCombatComponent* BossCombatComp;

	EKindOfBossMonster KindOfBossMonster;
	bool bSkillFlipflop;
	
	
public:
	void SetSkillFlipflop(bool Boolean) { bSkillFlipflop = Boolean; }
	bool GetSkillFlipflop() const { return bSkillFlipflop; }
	void SetKindOfBossMonster(EKindOfBossMonster Kind) { KindOfBossMonster = Kind; }
	EKindOfBossMonster GetKindOfBossMonster() const { return KindOfBossMonster; }
	void SetBossEgo(AMonsterCharacter* Ego) { BossEgo = Ego; }
	AMonsterCharacter* GetBossEgo() const { return BossEgo; }
	
};
