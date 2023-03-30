// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomBPFL.h"
#include "RoguelikeLoadingScreen/RoguelikeLoadingScreen.h"

void UCustomBPFL::PlayLoadingScreen(UUserWidget* Widget, bool PlayUntilStopped, float PlayTime)
{
	IRoguelikeLoadingScreenModule& LoadingScreenModule = IRoguelikeLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(Widget, PlayUntilStopped, PlayTime);
}

void UCustomBPFL::StopLoadingScreen()
{
	IRoguelikeLoadingScreenModule& LoadingScreenModule = IRoguelikeLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}

bool UCustomBPFL::IsInEditor()
{
	return GIsEditor;
}

void UCustomBPFL::FlushInput(const APlayerController* PC)
{
	if(PC && PC->PlayerInput)
	{
		PC->PlayerInput->FlushPressedKeys();
	}
}
