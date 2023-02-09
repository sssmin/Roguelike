// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/Item.h"
#include "SelectItemWidget.generated.h"

class UWidgetAnimation;
class UVerticalBox;
class USelectItemCellWidget;
class UButton;

DECLARE_DELEGATE_RetVal(TArray<FAllItemTable>, FCreateRandItem);


UCLASS()
class ROGUELIKE_API USelectItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(const TArray<FAllItemTable> SelectedItems);
	FCreateRandItem CreateRandItem;
	virtual void NativeConstruct() override;
	void CreateCellWidget();
private:
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* ShowAnim;

	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* LeftItemBox;

	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* RightItemBox;

	UPROPERTY(Meta = (BindWidget))
	UButton* F5Button;

	UPROPERTY(Meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USelectItemCellWidget> SelectItemCellWidgetClass;

	UFUNCTION()
	void F5ButtonClick();
	UFUNCTION()
	void ExitButtonClick();
	

	bool HaveEverPressed;

	TArray<USelectItemCellWidget*> CreatedCellWidgets;
	TArray<FAllItemTable> ItemInfo;
};
