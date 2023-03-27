// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TutorialInterface.generated.h"

class UTutorialManager;

UINTERFACE(MinimalAPI)
class UTutorialInterface : public UInterface
{
	GENERATED_BODY()
};


class ROGUELIKE_API ITutorialInterface
{
	GENERATED_BODY()

public:
	virtual void Enter() = 0;
	virtual void Execute(UTutorialManager* TutorialManager) = 0;
	virtual void Exit() = 0;
};
