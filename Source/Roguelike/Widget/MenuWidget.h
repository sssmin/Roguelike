// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UButton;

UCLASS()
class ROGUELIKE_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnClickedCloseButton();
	
	UPROPERTY(Meta = (BindWidget))
	UButton* OptionCloseButton;
};
