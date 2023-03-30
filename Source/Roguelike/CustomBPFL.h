// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomBPFL.generated.h"


UCLASS()
class ROGUELIKE_API UCustomBPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void PlayLoadingScreen(UUserWidget* Widget, bool PlayUntilStopped, float PlayTime);
	UFUNCTION(BlueprintCallable)
	static void StopLoadingScreen();
	UFUNCTION(BlueprintCallable)
	static bool IsInEditor();
	
private:
	UFUNCTION(BlueprintCallable)
	static void FlushInput(const APlayerController* PC);
};
