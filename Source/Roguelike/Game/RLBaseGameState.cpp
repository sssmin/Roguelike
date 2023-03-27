// Fill out your copyright notice in the Description page of Project Settings.
#include "RLBaseGameState.h"

#include "RLGameInstance.h"
#include "Kismet/GameplayStatics.h"


ARLBaseGameState::ARLBaseGameState()
{
	PrimaryActorTick.bCanEverTick = false;

	
}

void ARLBaseGameState::BeginPlay()
{
	Super::BeginPlay();

	RLGameInst = URLGameInstance::GetRLGameInst(this);
	
}
