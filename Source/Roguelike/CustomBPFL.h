// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomBPFL.generated.h"


UCLASS()
class ROGUELIKE_API UCustomBPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable)
	static void FlushInput(const APlayerController* PC);
};
