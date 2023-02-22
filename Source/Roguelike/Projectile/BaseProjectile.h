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
class APlayerCharacter;

UCLASS()
class ROGUELIKE_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	void SetCombatManage(const FCombatManager& InManager);
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* PMC;
	virtual void SetParticle();
	FCombatManager CombatManager; //�������� ���ݷ°� �Ӽ� �������.
	FItemManager ItemManager; //�������� ������
	UParticleSystem* HitParticle;
	UParticleSystem* ProjectileParticle;

private:	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* Sphere;

	void PlayHitEffect(); //ĳ���� ������ ��
	void PlayDestroyEffect(); //���� �°� �ı�������
	void CheckAttackerBeHealed(AActor* Other, APlayerCharacter* Player);
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	FVector StartLocation;
	float Range;
public:
	void SetVelocity(const FVector& Dir);
	void SetItemManager(FItemManager InManager) { ItemManager = InManager; }
	void SetRange(float InRange);
};
