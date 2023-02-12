// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLGameModeBase.h"
#include "Roguelike/Widget/MinimapWidget.h"
#include "Roguelike/Widget/MainUIWidget.h"
#include "Roguelike/Widget/SelectItemWidget.h"
#include "Roguelike/Widget/NoticeWidget.h"
#include "Roguelike/Actor/PlayersCamera.h"
#include "Roguelike/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void ARLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetActorTickEnabled(false);

}

void ARLPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ARLPlayerController::Init()
{
	bShowMouseCursor = true;
	FVector PlayerLocation = GetPawn()->GetActorLocation();
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, GetPawn()->GetActorLocation(), FVector(1.f, 1.f, 1.f));
	FActorSpawnParameters Params;
	Params.Owner = GetPawn();
	if (PlayersCameraClass && GetWorld())
	{
		CurrentPlayersCamera = GetWorld()->SpawnActor<APlayersCamera>(PlayersCameraClass, SpawnTransform, Params);
		if (CurrentPlayersCamera)
		{
			SetViewTargetWithBlend(CurrentPlayersCamera, 0.5f);
		}
	}
	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());

	if (MainUIWidgetClass)
	{
		MainUIWidget = CreateWidget<UMainUIWidget>(GetWorld(), MainUIWidgetClass);
		if (MainUIWidget)
		{
			MainUIWidget->AddToViewport();
		}
	}
	SetActorTickEnabled(true);
}

void ARLPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	LookAtCursor();
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

	if (CurrentPlayersCamera)
	{
		CurrentPlayersCamera->Destroy();
	}
	FVector PlayerLocation = GetPawn()->GetActorLocation();
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, GetPawn()->GetActorLocation(), FVector(1.f, 1.f, 1.f));
	FActorSpawnParameters Params;
	Params.Owner = GetPawn();
	if (PlayersCameraClass && GetWorld())
	{
		CurrentPlayersCamera = GetWorld()->SpawnActor<APlayersCamera>(PlayersCameraClass, SpawnTransform, Params);
		if (CurrentPlayersCamera)
		{
			SetViewTargetWithBlend(CurrentPlayersCamera, 0.7f);
		}
	}
	

}

void ARLPlayerController::DrawMap()
{
	if (MinimapWidgetClass && GetWorld())
	{
		MinimapWidget = CreateWidget<UMinimapWidget>(GetWorld(), MinimapWidgetClass);
		if (MinimapWidget)
		{
			MinimapWidget->Init(MapSize.X, MapSize.Y, Board);
			MinimapWidget->AddToViewport();
		}
	}
}

void ARLPlayerController::RemoveMinimapWidget()
{
	if (MinimapWidget)
	{
		MinimapWidget->RemoveFromViewport();
	}
}

void ARLPlayerController::ShowNoticeWidget(const FString& Notice)
{
	if (NoticeWidgetClass && GetWorld())
	{
		UNoticeWidget* NoticeWidget = CreateWidget<UNoticeWidget>(GetWorld(), NoticeWidgetClass);
		if (NoticeWidget)
		{
			NoticeWidget->AddToViewport();
			NoticeWidget->SetText(Notice);
		}
	}
}

void ARLPlayerController::ShowGameOverWidget()
{
	SetActorTickEnabled(false);
	
	if (GameOverWidgetClass && GetWorld())
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (GameOverWidget)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
			SetInputMode(InputModeData);
			
			GameOverWidget->AddToViewport();
		}
	}
}

void ARLPlayerController::ShowSelectItemWidget()
{
	SetActorTickEnabled(false);
	
	TArray<FItemInfoTable> SelectedItems = GetRandItem();
		
	if (SelectItemWidgetClass && GetWorld())
	{
		CreatedSelectItemWidget = CreateWidget<USelectItemWidget>(GetWorld(), SelectItemWidgetClass);
		if (CreatedSelectItemWidget)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
			SetInputMode(InputModeData);

			CreatedSelectItemWidget->AddToViewport();
			CreatedSelectItemWidget->Init(SelectedItems);
			CreatedSelectItemWidget->CreateCellWidget();
		}
	}
	
}

TArray<FItemInfoTable> ARLPlayerController::GetRandItem()
{
	TArray<FItemInfoTable> RandItem;
	ARLGameModeBase* GM = Cast<ARLGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		RandItem = GM->CreateRandItem();
	}
	return RandItem;
}

void ARLPlayerController::LookAtCursor()
{
	if (PlayerCharacter)
	{
		FVector Start = PlayerCharacter->GetActorLocation();
		FVector Target;
		FHitResult Hit;

		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			Target = Hit.ImpactPoint;
		}

		LookRot = UKismetMathLibrary::FindLookAtRotation(Start, Target);
		PlayerCharacter->SetLookRot(LookRot);
		PlayerCharacter->SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
	}
	
}

void ARLPlayerController::ResumeController()
{
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	SetInputMode(InputModeData);
	SetActorTickEnabled(true);
}

void ARLPlayerController::RemoveSelectWidget()
{
	if (CreatedSelectItemWidget)
	{
		CreatedSelectItemWidget->RemoveFromViewport();
	}
}

void ARLPlayerController::ActiveOnceItemListWidget(FItemInfoTable* SelectItem)
{
	if (MainUIWidget)
	{
		MainUIWidget->ItemListAnimPlay(SelectItem);
	}
}

void ARLPlayerController::RegisterItemEmptySlot(FItemInfoTable* Item)
{
	if (MainUIWidget)
	{
		MainUIWidget->RegisterItemEmptySlot(Item);
	}
}

void ARLPlayerController::DeactiveOnceItemListWidget()
{
	if (MainUIWidget)
	{
		MainUIWidget->DeactiveOnceItemListWidget();
	}
}

void ARLPlayerController::RequestItemSwap(const FItemInfoTable* OldItem, const FItemInfoTable* NewItem)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->RequestItemSwap(OldItem, NewItem);
	}
}

void ARLPlayerController::MoveMapFade()
{
	if (FadeWidgetClass && GetWorld())
	{
		CreateWidget<UUserWidget>(GetWorld(), FadeWidgetClass);
	}
}