// Fill out your copyright notice in the Description page of Project Settings.
#include "RLPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLGameModeBase.h"
#include "Roguelike/Widget/MinimapWidget.h"
#include "Roguelike/Widget/MainUIWidget.h"
#include "Roguelike/Widget/SelectItemWidget.h"
#include "Roguelike/Widget/NoticeWidget.h"
#include "Roguelike/Widget/RecallBarWidget.h"
#include "Roguelike/Actor/PlayersCamera.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Game/RLListenerManager.h"


void ARLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetActorTickEnabled(false);
	check(InputComponent);
	InputComponent->BindAction("ToggleMap", IE_Pressed, this, &ThisClass::ToggleMap);
	InputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	RecallTime = 5.f;
	bVisibleMap = false;
}

void ARLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && GI->GetListenerManager())
	{
		GI->GetListenerManager()->RestorePCDelegate.BindUObject(this, &ThisClass::RestorePC);
		GI->GetListenerManager()->RequestItemSwapDelegate.BindUObject(this, &ThisClass::RequestItemSwap);
		GI->GetListenerManager()->DeactiveOnceItemListDel.BindUObject(this, &ThisClass::DeactiveOnceItemListWidget);
		GI->GetListenerManager()->GetRandItemDelegate.BindUObject(this, &ThisClass::GetRandItem);
	}
}

void ARLPlayerController::Init()
{
	bShowMouseCursor = true;
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, GetPawn()->GetActorLocation(), FVector(1.f, 1.f, 1.f));
	FActorSpawnParameters Params;
	Params.Owner = GetPawn();
	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayersCameraClass && GetWorld())
	{
		CurrentPlayersCamera = GetWorld()->SpawnActor<APlayersCamera>(PlayersCameraClass, SpawnTransform, Params);
		if (CurrentPlayersCamera)
		{
			SetViewTargetWithBlend(CurrentPlayersCamera, 0.5f);
		}
	}

	if (MainUIWidgetClass)
	{
		MainUIWidget = CreateWidget<UMainUIWidget>(GetWorld(), MainUIWidgetClass);
		if (MainUIWidget)
		{
			MainUIWidget->AddToViewport();
			MainUIWidget->SetOwner(GetPawn());
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

void ARLPlayerController::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		GetPawn()->AddMovementInput(Direction, Value);
		if (GetWorld()->GetTimerManager().IsTimerActive(RecallTimerHandle))
		{
			RecallCancel();
		}
	}
}

void ARLPlayerController::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		GetPawn()->AddMovementInput(Direction, Value);
		if (GetWorld()->GetTimerManager().IsTimerActive(RecallTimerHandle))
		{
			RecallCancel();
		}
	}
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
			bVisibleMap ? MinimapWidget->SetVisibility(ESlateVisibility::Visible) : MinimapWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ARLPlayerController::ToggleMap()
{
	if (MinimapWidget)
	{
		if (bVisibleMap)
		{
			MinimapWidget->SetVisibility(ESlateVisibility::Hidden);
			bVisibleMap = false;
		}
		else
		{
			MinimapWidget->SetVisibility(ESlateVisibility::Visible);
			bVisibleMap = true;
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
	StopFire();
	
	if (GameOverWidgetClass && GetWorld())
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (GameOverWidget)
		{
			FInputModeUIOnly InputModeData;
			//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
			SetInputMode(InputModeData);
			
			GameOverWidget->AddToViewport();
		}
	}
}

void ARLPlayerController::ShowSelectItemWidget()
{
	SetActorTickEnabled(false);
	StopFire();
	TArray<UItemInfo*> SelectedItems = GetRandItem();
		
	if (SelectItemWidgetClass && GetWorld())
	{
		CreatedSelectItemWidget = CreateWidget<USelectItemWidget>(GetWorld(), SelectItemWidgetClass);
		if (CreatedSelectItemWidget)
		{
			FInputModeUIOnly InputModeData;
			SetInputMode(InputModeData);

			CreatedSelectItemWidget->AddToViewport();
			CreatedSelectItemWidget->Init(SelectedItems);
			CreatedSelectItemWidget->CreateCellWidget();
		}
	}
	PlayerInput->FlushPressedKeys();
}

TArray<UItemInfo*> ARLPlayerController::GetRandItem()
{
	TArray<UItemInfo*> RandItem;
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
			LookRot = UKismetMathLibrary::FindLookAtRotation(Start, Target);
			PlayerCharacter->SetLookRot(LookRot);
			PlayerCharacter->SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
		}
	}
}

void ARLPlayerController::ResumeController()
{
	FInputModeGameAndUI InputModeData;
	//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
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

void ARLPlayerController::ActiveOnceItemListWidget(UItemInfo* SelectItem) const
{
	if (MainUIWidget && SelectItem)
	{
		MainUIWidget->ItemListAnimPlay(SelectItem);
	}
}

void ARLPlayerController::RegisterItemEmptySlot(UItemInfo* Item) const
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

void ARLPlayerController::RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem)
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

void ARLPlayerController::StopFire()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->StopFire();
	}
}

void ARLPlayerController::RecallStart()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(RecallTimerHandle))
	{
		return;
	}
	
	if (RecallWidgetClass)
	{
		RecallBarWidget = CreateWidget<URecallBarWidget>(GetWorld(), RecallWidgetClass);
		if (RecallBarWidget)
		{
			GetWorld()->GetTimerManager().ClearTimer(RecallTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(RecallTimerHandle, this, &ThisClass::RecallTimerFinished, RecallTime, false);
			RecallBarWidget->AddToViewport();
		}
	}
	if (RecallStartParticle)
	{
		RecallStartParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RecallStartParticle, GetPawn()->GetActorTransform());
	}
}

void ARLPlayerController::RecallTimerFinished()
{
	RecallCompleteSuccessfully.ExecuteIfBound();
	RecallEnd();
}

void ARLPlayerController::RecallCancel()
{
	if (RecallBarWidget)
	{
		RecallBarWidget->RemoveFromParent();
		GetWorld()->GetTimerManager().ClearTimer(RecallTimerHandle);
	}
	if (RecallStartParticleComp)
	{
		RecallStartParticleComp->DestroyComponent();
	}
}

void ARLPlayerController::RecallEnd()
{
	RecallCancel();
	if (RecallEndParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RecallEndParticle, GetPawn()->GetActorTransform());
	}
}

void ARLPlayerController::InitOnceItemWidget()
{
	if (MainUIWidget)
	{
		MainUIWidget->InitOnceItemList();
	}
}

void ARLPlayerController::RestorePC()
{
	ResumeController();
	RemoveSelectWidget();
	DeactiveOnceItemListWidget();
}