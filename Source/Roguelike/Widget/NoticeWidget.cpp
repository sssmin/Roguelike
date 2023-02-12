// Fill out your copyright notice in the Description page of Project Settings.


#include "NoticeWidget.h"
#include "Components/TextBlock.h"

void UNoticeWidget::SetText(const FString& Text)
{
	if (Notice)
	{
		Notice->SetText(FText::FromString(Text));
	}
}