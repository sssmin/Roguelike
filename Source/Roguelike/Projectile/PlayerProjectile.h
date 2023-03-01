// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "PlayerProjectile.generated.h"

class UParticleSystem;

UCLASS()
class ROGUELIKE_API APlayerProjectile : public ABaseProjectile
{
	GENERATED_BODY()
public:
	APlayerProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UParticleSystem*> ProjectileParticles;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UParticleSystem*> HitParticles;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UParticleSystem*> DestroyParticles;
	
};
