// Fill out your copyright notice in the Description page of Project Settings.
#include "RLPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Roguelike/Roguelike.h"

#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLMainGameMode.h"
#include "Roguelike/Widget/MinimapWidget.h"
#include "Roguelike/Widget/MainUIWidget.h"
#include "Roguelike/Widget/SelectItemWidget.h"
#include "Roguelike/Widget/NoticeWidget.h"
#include "Roguelike/Widget/RecallBarWidget.h"
#include "Roguelike/Widget/StatNoticeWidget.h"
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

	URLGameInstance* GI = Cast<URLGameInstance>(GetGameInstance());
	if (GI && GI->GetListenerManager())
	{
		GI->GetListenerManager()->RestorePCDelegate.BindUObject(this, &ThisClass::RestorePC);
		GI->GetListenerManager()->RequestItemSwapDelegate.BindUObject(this, &ThisClass::RequestItemSwap);
		GI->GetListenerManager()->DeactivateOnceItemListDel.BindUObject(this, &ThisClass::DeactivateOnceItemListWidget);
		GI->GetListenerManager()->GetRandItemDelegate.BindUObject(this, &ThisClass::GetRandItem);
		GI->GetListenerManager()->OnNewGameDelegate.AddUObject(this, &ThisClass::Init);
		GI->GetListenerManager()->OnStartTutorialDelegate.AddUObject(this, &ThisClass::Init);
		GI->GetListenerManager()->OnLoadGameDelegate.AddUObject(this, &ThisClass::LoadGame);
	}
}

void ARLPlayerController::Init()
{
	bShowMouseCursor = true;
	const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, GetPawn()->GetActorLocation(), FVector(1.f, 1.f, 1.f));
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

void ARLPlayerController::LoadGame()
{
	Init();
	URLGameInstance* GI = Cast<URLGameInstance>(GetGameInstance());
	if (GI)
	{
		LoadItem(GI->GetItemInfos());
		GetPawn()->SetActorTransform(GI->GetTempPlayerTransform(), false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void ARLPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	LookAtCursor();
}

void ARLPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	URLGameInstance* GI = Cast<URLGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->RequestInfo();
	}
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
	DrawMap(Size, InBoard);
	SetViewTargetWithBlend(nullptr, 0.f);
	
	if (CurrentPlayersCamera)
	{
		SetViewTargetWithBlend(CurrentPlayersCamera, 0.7f);
	}
}

void ARLPlayerController::DrawMap(FVector2Int Size, TArray<FCell> InBoard)
{
	if (MinimapWidgetClass && GetWorld())
	{
		MinimapWidget = CreateWidget<UMinimapWidget>(GetWorld(), MinimapWidgetClass);
		if (MinimapWidget)
		{
			MinimapWidget->Init(Size.X, Size.Y, InBoard);
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

void ARLPlayerController::RemoveMinimapWidget() const
{
	if (MinimapWidget)
	{
		MinimapWidget->RemoveFromParent();
	}
}

void ARLPlayerController::ShowNoticeWidget(const FString& Notice) const
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
			SetInputMode(InputModeData);
			
			GameOverWidget->AddToViewport();
		}
	}
}

void ARLPlayerController::ShowSelectItemWidget(bool IsTutorial)
{
	SetActorTickEnabled(false);
	StopFire();
	const TArray<UItemInfo*> SelectedItems = GetRandItem();
		
	if (SelectItemWidgetClass && GetWorld())
	{
		CreatedSelectItemWidget = CreateWidget<USelectItemWidget>(GetWorld(), SelectItemWidgetClass);
		if (CreatedSelectItemWidget)
		{
			FInputModeUIOnly InputModeData;
			SetInputMode(InputModeData);

			CreatedSelectItemWidget->AddToViewport();
			CreatedSelectItemWidget->Init(SelectedItems, IsTutorial);
			CreatedSelectItemWidget->CreateCellWidget();
		}
	}
	PlayerInput->FlushPressedKeys();
}

void ARLPlayerController::DeactivateSelectItemWidget()
{
	if (CreatedSelectItemWidget)
	{
		CreatedSelectItemWidget->Deactivate();
	}
}

TArray<UItemInfo*> ARLPlayerController::GetRandItem() const
{
	TArray<UItemInfo*> RandItem;
	ARLBaseGameMode* GM = Cast<ARLBaseGameMode>(UGameplayStatics::GetGameMode(this));
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
		const FVector Start = PlayerCharacter->GetActorLocation();
		FVector Target = FVector();
		FHitResult Hit;

		GetHitResultUnderCursor(ECC_CursorTrace, false, Hit);
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
	SetInputMode(InputModeData);
	SetActorTickEnabled(true);
}

void ARLPlayerController::RemoveSelectWidget() const
{
	if (CreatedSelectItemWidget)
	{
		CreatedSelectItemWidget->RemoveFromParent();
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

void ARLPlayerController::DeactivateOnceItemListWidget() const
{
	if (MainUIWidget)
	{
		MainUIWidget->DeactivateOnceItemListWidget();
	}
}

void ARLPlayerController::RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem) const
{
	if (PlayerCharacter)
	{
		PlayerCharacter->RequestItemSwap(OldItem, NewItem);
	}
}

void ARLPlayerController::MoveMapFade() const
{
	if (FadeWidgetClass && GetWorld())
	{
		CreateWidget<UUserWidget>(GetWorld(), FadeWidgetClass);
	}
}

void ARLPlayerController::StopFire() const
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

void ARLPlayerController::InitOnceItemWidget() const
{
	if (MainUIWidget)
	{
		MainUIWidget->InitOnceItemList();
	}
}

void ARLPlayerController::SetStateIcon(EState State, UTexture2D* Icon) const
{
	if (MainUIWidget)
	{
		MainUIWidget->SetStateIcon(State, Icon);
	}
}

void ARLPlayerController::RemoveStateIcon(EState State) const
{
	if (MainUIWidget)
	{
		MainUIWidget->RemoveStateIcon(State);
	}
}

void ARLPlayerController::FlickerStateIcon(EState State) const
{
	if (MainUIWidget)
	{
		MainUIWidget->FlickerStateIcon(State);
	}
}

void ARLPlayerController::SetBuffIcon(EBuff Buff, UTexture2D* Icon) const
{
	if (MainUIWidget)
	{
		MainUIWidget->SetBuffIcon(Buff, Icon);
	}
}

void ARLPlayerController::RemoveBuffIcon(EBuff Buff) const
{
	if (MainUIWidget)
	{
		MainUIWidget->RemoveBuffIcon(Buff);
	}
}

void ARLPlayerController::FlickerBuffIcon(EBuff Buff) const
{
	if (MainUIWidget)
	{
		MainUIWidget->FlickerBuffIcon(Buff);
	}
}

void ARLPlayerController::ShowStatNoticeWidget(EINFStackItem StatItem, const FString& StatName, const float CurrentValue, const float IncreaseValue)
{
	ARLMainGameMode* GM = Cast<ARLMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && StatNoticeClass)
	{
		UTexture2D* Icon = GM->GetStatImage(StatItem);
		if (Icon)
		{
			UStatNoticeWidget* StatNotice = CreateWidget<UStatNoticeWidget>(GetWorld(), StatNoticeClass);
			if (StatNotice)
			{
				StatNotice->AddToViewport();
				StatNotice->SetStatNotice(StatName, CurrentValue, IncreaseValue, Icon);
			}
		}
	}
}

void ARLPlayerController::RestorePC()
{
	ResumeController();
	RemoveSelectWidget();
	DeactivateOnceItemListWidget();
}

void ARLPlayerController::LoadItem(TArray<UItemInfo*> ItemInfos)
{
	ARLMainGameMode* GM = Cast<ARLMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->SetItemIcon(ItemInfos);
	}
	if (MainUIWidget)
	{
		MainUIWidget->LoadItem(ItemInfos);
	}
}
