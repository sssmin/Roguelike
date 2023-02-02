// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameModeBase.h"

ARLGameModeBase::ARLGameModeBase()
{

}

void ARLGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARLGameModeBase::SpawnMob(int32 StageLevel, int32 MobCount)
{
	//몹 스폰
}

void ARLGameModeBase::SpawnBoss(int32 StageLevel)
{
	//보스 스폰
}