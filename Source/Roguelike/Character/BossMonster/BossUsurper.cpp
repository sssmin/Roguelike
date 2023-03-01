// Fill out your copyright notice in the Description page of Project Settings.
#include "BossUsurper.h"

void ABossUsurper::ExecuteSkill()
{
	if (GetSkillFlipflop())
	{
		int32 RandValue = FMath::RandRange(1, 4);
		if (RandValue == 1)
		{
			Whirlwind();
		}
		else
		{
			Breath();
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

void ABossUsurper::SpecialAttack(AActor* Target)
{
	ABossMonsterCharacter::SpecialAttack(Target);
}
