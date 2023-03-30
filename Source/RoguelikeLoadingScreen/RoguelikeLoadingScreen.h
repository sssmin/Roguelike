#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class UUserWidget;

class IRoguelikeLoadingScreenModule : public IModuleInterface
{
public:
	static inline IRoguelikeLoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IRoguelikeLoadingScreenModule>("RoguelikeLoadingScreen");
	}

	virtual void StartInGameLoadingScreen(UUserWidget* Widget, bool bPlayUntilStopped, float PlayTime) = 0;

	virtual void StopInGameLoadingScreen() = 0;
};