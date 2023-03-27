// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Roguelike/Roguelike.h"
#include "GameFramework/Actor.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;
class ABaseCharacter;

UCLASS()
class ROGUELIKE_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void SetCombatManage(const FCombatManager& InManager);
	virtual void SetVelocity(const FVector& Dir);
	void SetRange(float InRange);
	
protected:
	virtual void SpawnParticle();
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void PlayHitEffect();
	void PlayDestroyEffect();
	void CheckAttackerBeHealed(ABaseCharacter* Attacker);
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* PMC;
	FCombatManager CombatManager;
	FItemManager ItemManager;

private:	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ProjectileParticle;
	UPROPERTY()
	TSubclassOf<UDamageType> ProjectileDamageType;
	
	FVector StartLocation;
	float Range;
	
public:
	void SetItemManager(FItemManager InManager) { ItemManager = InManager; }
	void SetProjectileParticle(UParticleSystem* Particle) { ProjectileParticle = Particle; }
	UParticleSystem* GetProjectileParticle() const { return ProjectileParticle; }
	void SetHitParticle(UParticleSystem* Particle) { HitParticle = Particle; }
	UParticleSystem* GetHitParticle() const { return HitParticle; }
	void SetDamageType(TSubclassOf<UDamageType> Type) { ProjectileDamageType = Type; }
	TSubclassOf<UDamageType> GetDamageType() const { return ProjectileDamageType; }
	USphereComponent* GetSphereComp() const { return Sphere; }
	
};
