#include "MinimapWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

#include "MinimapCellWidget.h"

void UMinimapWidget::Init(int32 MapSizeX, int32 MapSizeY, TArray<FCell> Board)
{
	if (MapGrid && MinimapCellWidgetClass && GetWorld())
	{
		for (int i = 0; i < MapSizeX; i++)
		{
			for (int j = 0; j < MapSizeY; j++)
			{
				int32 CurrentIndex = i * MapSizeY + j;

				CellWidgets.Add(CreateWidget<UMinimapCellWidget>(GetWorld(), MinimapCellWidgetClass));
				CellWidgets[CurrentIndex]->AddToViewport();
				MapGrid->AddChildToUniformGrid(CellWidgets[CurrentIndex], i, j);
				CellWidgets[CurrentIndex]->SetVisibility(ESlateVisibility::Visible);
				CellWidgets[CurrentIndex]->Init(Board[CurrentIndex].CellState);
			}
		}
	}
}
