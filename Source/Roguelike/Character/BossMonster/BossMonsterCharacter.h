// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "BossMonsterCharacter.generated.h"

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
	virtual void ExecuteSkill();
	virtual void SpecialAttack(AActor* Target) override;
	virtual void GiveBTToController() override;
	void OnSkillEnd();
	void OnExecuteBreath();
	void OnExecuteThrowBall();
	void OnExecuteWhirlwind();
	void OnExecuteSquare();
protected:
	virtual void Whirlwind();
	virtual void ThrowBall();
	virtual void Breath();
	virtual void Square();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWhirlwindActor> WhirlwindActorClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABreathActor> BreathActorClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AThrowBallProjectile> ThrowProjectileClass;

	EKindOfBossMonster KindOfBossMonster;

	bool bSkillFlipflop;
	bool bIsActiveBreath;
	int32 SpawnedBreathNum;
	float BreathSpawnTime;
	float BreathDegree;
	int32 BreathMaxSpawnNum;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BreathMontage;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ThrowMontage;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* WhirlwindMontage;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SquareMontage;

	FVector GetRandomFloor();
	void SpawnBreathActor();
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BossBT;
	FVector GetRandomLoc(const FVector& CellScale);
	FVector GetRandomDir(const FVector& CellScale, const FVector& RandomLoc);
	void FireToDir(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType);

public:
	void SetSkillFlipflop(bool Boolean) { bSkillFlipflop = Boolean; }
	bool GetSkillFlipflop() const { return bSkillFlipflop; }
	void SetKindOfBossMonster(EKindOfBossMonster Kind) { KindOfBossMonster = Kind; }
	EKindOfBossMonster GetKindOfBossMonster() const { return KindOfBossMonster; }
	
};
