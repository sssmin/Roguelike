// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseSkillActor.h"

ABaseSkillActor::ABaseSkillActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseSkillActor::BeginPlay()
{
	Super::BeginPlay();
}