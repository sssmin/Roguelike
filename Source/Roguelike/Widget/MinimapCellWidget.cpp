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
		case ECellState::DEACTIVE:
			Cell->SetBrushColor(Color);
			break;
		case ECellState::NORMAL:
			Cell->SetBrushColor(FLinearColor::White);
			break;
		case ECellState::BONUS:
			Cell->SetBrushColor(FLinearColor::Yellow);
			break;
		case ECellState::DONT_FIND_BOSS:
			Cell->SetBrushColor(FLinearColor::White);
			break;
		case ECellState::DISCOVERED_BOSS:
			Cell->SetBrushColor(FLinearColor::Red);
			break;
		case ECellState::IN_PLAYER:
			Cell->SetBrushColor(FLinearColor::Black);
			if (FlikerAnim)
			{
				PlayAnimation(FlikerAnim, 0.f, 0);
			}
			break;
		case ECellState::CLEAR:
			Cell->SetBrushColor(FLinearColor::Gray);
			break;

		}
		Cell->SetRenderOpacity(0.7f);
	}
}


