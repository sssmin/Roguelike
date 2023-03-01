// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "MonsterSkeleton.generated.h"

class ABombProjectile;

UCLASS()
class ROGUELIKE_API AMonsterSkeleton : public AMonsterCharacter
{
	GENERATED_BODY()
	
public:
	AMonsterSkeleton();
	virtual void BeginPlay() override;
	virtual void GiveBTToController() override;
protected:
	virtual void SpecialAttack(AActor* Target) override;
private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* SkeletonBT;
	
};
