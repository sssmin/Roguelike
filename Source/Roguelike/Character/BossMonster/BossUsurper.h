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
	ABossUsurper();
	virtual void ExecuteSkill() override;

protected:
	virtual void Destroyed() override;
	
};
