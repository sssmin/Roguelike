// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Widget/MinimapWidget.h"
#include "Roguelike/PlayersCamera.h"



void ARLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void ARLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	FVector PlayerLocation = GetPawn()->GetActorLocation();
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, GetPawn()->GetActorLocation(), FVector(1.f, 1.f, 1.f));
	FActorSpawnParameters Params;
	Params.Owner = GetPawn();
	if (PlayersCameraClass)
	{
		APlayersCamera* PlayersCamera = GetWorld()->SpawnActor<APlayersCamera>(PlayersCameraClass, SpawnTransform, Params);
		if (PlayersCamera)
		{
			SetViewTargetWithBlend(PlayersCamera);
		}
	}
	

}

void ARLPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	Cast<URLGameInstance>(GetGameInstance())->RequestInfo();
}

void ARLPlayerController::SetMapInfo(FVector2Int Size, TArray<FCell> InBoard, int32 PlayerCurrentCell)
{
	MapSize = Size;
	Board = InBoard;
	PlayerCell = PlayerCurrentCell;
	DrawMap();
}

void ARLPlayerController::DrawMap()
{
	if (MinimapWidgetClass)
	{
		MinimapWidget = CreateWidget<UMinimapWidget>(GetWorld(), MinimapWidgetClass);
		if (MinimapWidget)
		{
			MinimapWidget->Init(MapSize.X, MapSize.Y, Board);
			MinimapWidget->AddToViewport();

		}
	}
	
}

