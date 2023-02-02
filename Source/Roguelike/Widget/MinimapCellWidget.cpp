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
		case ECellState::DONTFINDBOSS:
			Cell->SetBrushColor(FLinearColor::White);
			break;
		case ECellState::DISCOVEREDBOSS:
			Cell->SetBrushColor(FLinearColor::Red);
			break;
		case ECellState::INPLAYER:
			//Cell->SetBrushColor(FLinearColor::White);
			if (LocationImg)
			{
				Cell->SetBrushFromTexture(LocationImg);
			}
			break;
		case ECellState::CLEAR:
			Cell->SetBrushColor(FLinearColor::Gray);
			break;

		}
	}
}


