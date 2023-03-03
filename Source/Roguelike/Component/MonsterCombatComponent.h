// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "MonsterCombatComponent.generated.h"

class AMonsterProjectile;
class AOnetoAnotherProjectile;
class ABombProjectile;
class ATurretProjectile;

UCLASS()
class ROGUELIKE_API UMonsterCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
	
public:
	UMonsterCombatComponent();
	void FireVariousProjectile(int32 Parts, TSubclassOf<UDamageType> DamageType);
	void RequestHeal();
	void FireOneToTwo(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType);
	void ThrowBomb(AActor* Target, TSubclassOf<UDamageType> DamageType);
	virtual void FireToDir(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType);
	
	void FireInParts(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType);
	void FireSpreadFromCenter(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType);
	
	static UMonsterCombatComponent* GetMonsterCombatComp(AActor* Character);
	
private:
	FVector CalcThrowVector(AActor* Target);

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOnetoAnotherProjectile> OnetoAnotherProjectileClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABombProjectile> BombProjectileClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATurretProjectile> TurretProjectileClass;
	UPROPERTY()
	AActor* Turret;


	
public:
	void SetTurret(AActor* Actor) { Turret = Actor; }
	TSubclassOf<AMonsterProjectile> GetProjectileClass() const { return ProjectileClass; }
};
