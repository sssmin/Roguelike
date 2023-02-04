// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Roguelike/Roguelike.h"
#include "GameFramework/Actor.h"
#include "Roguelike/Manage.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;

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

	void PlayHitEffect(); //ĳ���� ������ ��
	void PlayDestroyEffect(); //���� �°� �ı�������

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Ư��, ����� ���� ���� �� �ְ���.
	FCombatManage CombatManage; //�������� ���ݷ°� �Ӽ� �������.

public:
	void SetVelocity(const FVector& Dir);
	void SetCombatManage(FCombatManage InManage) { CombatManage = InManage; }
};
