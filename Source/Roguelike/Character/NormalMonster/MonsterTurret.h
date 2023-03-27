// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "MonsterTurret.generated.h"

class USoundCue;

UCLASS()
class ROGUELIKE_API AMonsterTurret : public AMonsterCharacter
{
	GENERATED_BODY()
	
public:
	AMonsterTurret();
	virtual void GiveBTToController() override;
	virtual void SpecialAttack(AActor* Target) override;
	virtual void Destroyed() override;


private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* TurretBT;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USoundCue* DestroySoundCue;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DestroyParticle;
	
};
