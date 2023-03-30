// Fill out your copyright notice in the Description page of Project Settings.
#include "BossSoulEater.h"

ABossSoulEater::ABossSoulEater()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABossSoulEater::BeginPlay()
{
	Super::BeginPlay();
}

void ABossSoulEater::ExecuteSkill()
{
	if (GetSkillFlipflop())
	{
		const int32 RandValue = FMath::RandRange(1, 4);
		if (RandValue == 1)
		{
			Whirlwind();
		}
		else
		{
			BreathForward();
		}
		SetSkillFlipflop(false);
	}
	else
	{
		if (FMath::RandBool())
		{
	
			ThrowBall();
		}
		else
		{
			Square();
		}
		SetSkillFlipflop(true);
	}
}
