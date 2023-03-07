// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MonsterCombatComponent.h"
#include "BossCombatComponent.generated.h"

class AWhirlwindActor;
class ABreathActor;
class AThrowBallProjectile;


enum class EBreathType : uint8
{
	NONE,
	RADIAL,
	FORWARD
};

UCLASS()
class ROGUELIKE_API UBossCombatComponent : public UMonsterCombatComponent
{
	GENERATED_BODY()

public:
	UBossCombatComponent();
	void ThrowBall();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	void Breath(EBreathType Type, FVector Location);
	void Whirlwind();
	void RemoveSpawnedWhirlwindActor(AWhirlwindActor* WhirlwindActor);
	void ReserveDestroySpawnedWhirlwind();
	virtual void FireToDir(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType) override;
	void Square();
	
private:
	void SpawnBreathActor();
	FVector GetRandomFloor() const;
	FVector GetRandomLoc(const FVector& CellScale) const;
	FVector GetRandomDir(const FVector& CellScale, const FVector& RandomLoc);
	void FireThrowBall(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType) const;
	void DeactivateBreath();
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWhirlwindActor> WhirlwindActorClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABreathActor> BreathActorClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AThrowBallProjectile> ThrowProjectileClass;
	UPROPERTY()
	TArray<AWhirlwindActor*> SpawnedWhirlwindActors;
	
	bool bIsActiveBreath;
	EBreathType BreathType;
	int32 SpawnedBreathNum;
	float BreathSpawnTime;
	float BreathDegree;
	int32 BreathMaxSpawnNum;
	int32 WhirlwindMaxNum;
	FVector BreathSpawnLocaton;
	
};

