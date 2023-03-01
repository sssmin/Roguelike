// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseSkillActor.h"

ABaseSkillActor::ABaseSkillActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseSkillActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseSkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

