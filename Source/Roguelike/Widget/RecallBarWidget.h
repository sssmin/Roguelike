// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecallBarWidget.generated.h"


UCLASS()
class ROGUELIKE_API URecallBarWidget : public UUserWidget
{
	GENERATED_BODY()

	float Per;
	FString Time;
	
};
