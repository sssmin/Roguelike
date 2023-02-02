// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* PMC;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ProjectileParticle;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//특성, 대미지 등을 넣을 수 있겠음.

public:
	void SetVelocity(const FVector& Dir);
};
