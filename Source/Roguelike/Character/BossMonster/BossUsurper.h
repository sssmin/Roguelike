// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BossMonsterCharacter.h"
#include "BossUsurper.generated.h"


UCLASS()
class ROGUELIKE_API ABossUsurper : public ABossMonsterCharacter
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteSkill() override;
	virtual void SpecialAttack(AActor* Target) override;
	
};
