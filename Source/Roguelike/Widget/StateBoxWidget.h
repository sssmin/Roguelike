// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/StatManage.h"
#include "StateBoxWidget.generated.h"

class UHorizontalBox;
class UPlayerStateWidget;
UCLASS()
class ROGUELIKE_API UStateBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStateIcon(EState State, UTexture2D* Icon);
	void SetBuffIcon(EBuff Buff, UTexture2D* Icon);
	void FlickerStateIcon(EState State);
	void FlickerBuffIcon(EBuff Buff);
	void RemoveStateIcon(EState State);
	void RemoveBuffIcon(EBuff State);
	
private:
	UPROPERTY(Meta = (BindWidget))
	UHorizontalBox* StateBox;
	
	TMap<EState, UPlayerStateWidget*> States;
	TMap<EBuff, UPlayerStateWidget*> Buffs;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPlayerStateWidget> PlayerStateWidgetClass;
	
};
