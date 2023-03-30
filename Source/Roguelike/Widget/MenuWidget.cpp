// Fill out your copyright notice in the Description page of Project Settings.
#include "MenuWidget.h"

#include "Components/Button.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OptionCloseButton)
	{
		OptionCloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCloseButton);
	}
}

void UMenuWidget::OnClickedCloseButton()
{
	ConditionalBeginDestroy();
}
