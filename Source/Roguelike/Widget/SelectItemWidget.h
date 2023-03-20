// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/ItemManage.h"
#include "SelectItemWidget.generated.h"

class UWidgetAnimation;
class UVerticalBox;
class USelectItemCellWidget;
class UButton;
class UItemComponent;


UCLASS()
class ROGUELIKE_API USelectItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(const TArray<UItemInfo*> SelectedItems, bool IsTutorial);
	virtual void NativeConstruct() override;
	void CreateCellWidget();
	void Deactivate();
	
private:
	UFUNCTION()
	void F5ButtonClick();
	UFUNCTION()
	void ExitButtonClick();
	void Refresh();
	
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
	UPROPERTY()
	TArray<USelectItemCellWidget*> CreatedCellWidgets;
	UPROPERTY()
	TArray<UItemInfo*> ItemInfo;
	
	bool bHaveEverPressed;
	bool bIsTutorial;
	
};
