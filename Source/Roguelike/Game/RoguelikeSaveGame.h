// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RoguelikeSaveGame.generated.h"


UCLASS()
class ROGUELIKE_API URoguelikeSaveGame : public USaveGame
{
	GENERATED_BODY()

private:
	
	FString SaveSlotName;
	int32 SaveIndex;

	
};
