// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomBPFL.h"

void UCustomBPFL::FlushInput(const APlayerController* PC)
{
	if(PC && PC->PlayerInput)
	{
		PC->PlayerInput->FlushPressedKeys();
	}
}
