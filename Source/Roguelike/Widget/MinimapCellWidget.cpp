// Fill out your copyright notice in the Description page of Project Settings.
#include "MinimapCellWidget.h"
#include "Components/Border.h"

void UMinimapCellWidget::Init(ECellState CellState)
{
	if (Cell)
	{
		FLinearColor Color;
		Color.A = 0;
		switch (CellState)
		{
		case ECellState::Deactivate:
			Cell->SetBrushColor(Color);
			break;
		case ECellState::Normal:
			Cell->SetBrushColor(FLinearColor::White);
			break;
		case ECellState::Bonus:
			Cell->SetBrushColor(FLinearColor::Yellow);
			break;
		case ECellState::DontFindBoss:
			Cell->SetBrushColor(FLinearColor::White);
			break;
		case ECellState::DiscoveredBoss:
			Cell->SetBrushColor(FLinearColor::Red);
			break;
		case ECellState::InPlayer:
			Cell->SetBrushColor(FLinearColor::Black);
			if (FlikerAnim)
			{
				PlayAnimation(FlikerAnim, 0.f, 0);
			}
			break;
		case ECellState::Clear:
			Cell->SetBrushColor(FLinearColor::Gray);
			break;

		}
		Cell->SetRenderOpacity(0.7f);
	}
}


