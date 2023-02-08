// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Roguelike/Roguelike.h"
#include "GameFramework/Actor.h"
#include "Roguelike/Type/Manage.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;
class APlayerCharacter;

UCLASS()
class ROGUELIKE_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* PMC;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UParticleSystem*> ProjectileParticles;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UParticleSystem*> HitParticles;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UParticleSystem*> DestroyParticles;

	void PlayHitEffect(); //캐릭터 맞췄을 때
	void PlayDestroyEffect(); //벽에 맞고 파괴됐을때
	void CheckAttackerBeHealed(AActor* Other, APlayerCharacter* Player);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FCombatManage CombatManage; //공격자의 공격력과 속성 들어있음.
	FItemManage ItemManage; //공격자의 아이템

public:
	void SetVelocity(const FVector& Dir);
	void SetCombatManage(FCombatManage InManage) { CombatManage = InManage; }
	void SetItemManage(FItemManage InManage) { ItemManage = InManage; }
};
