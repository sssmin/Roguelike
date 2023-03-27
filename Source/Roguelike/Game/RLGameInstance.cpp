// Fill out your copyright notice in the Description page of Project Settings.
#include "RLGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/InputSettings.h"
#include "Serialization/BufferArchive.h"

#include "Roguelike/Map/DFSAgrt.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "RLListenerManager.h"
#include "RLMainGameState.h"
#include "RLTutorialGameMode.h"
#include "RoguelikeSaveGame.h"
#include "TutorialManager.h"


URLGameInstance::URLGameInstance()
{
	LoadState = ELoadState::None;
	AxisRollbackCount = 0;
}

void URLGameInstance::Init()
{
	Super::Init();
	
	DFS = MakeShared<DFSAgrt>();
	ListenerManager = NewObject<URLListenerManager>();
	Initialize(0);
}

void URLGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	Super::LoadComplete(LoadTime, MapName);
	
	check(ListenerManager);
	
	if (MapName == "/Game/Maps/GameMap")
	{
		switch (LoadState)
		{
		case ELoadState::NewGame:
			ListenerManager->OnNewGame();
			break;
		case ELoadState::LoadGame:
			ListenerManager->OnLoadGame();
			break;
		}
	}
	else if (MapName == "/Game/Maps/TutorialMap")
	{
		//튜토리얼 게임모드랑 내통. 튜토리얼  첫시작임.
		ARLTutorialGameMode* RLTutoGM = Cast<ARLTutorialGameMode>(UGameplayStatics::GetGameMode(this));
		if (RLTutoGM)
		{
			ListenerManager->OnStartTutorial();
		}
	}
}

void URLGameInstance::StartTutorial()
{
	Initialize(0);
	StageLevel = 0;
	GenerateTutorialMap();
}

void URLGameInstance::NewGame()
{
	Initialize(1);
	GenerateMap();
	LoadState = ELoadState::NewGame;
}

void URLGameInstance::Initialize(int32 Level)
{
	Board.Empty();
	StartPos = FVector::ZeroVector;
	StartCell = 0;
	PlayerCurrentCell = 0;
	StageLevel = Level;
	BossCell = 0;
	BossPrevCell = 0;

	TotalCellNum = 0;

	TempHealthManager = FHealthManager();
	TempCombatManager = FCombatManager();
	TempBuff = 0;
	
	RLGameState = Cast<ARLMainGameState>(UGameplayStatics::GetGameState(this));
}

void URLGameInstance::GenerateMap()
{
	if (DFS.IsValid())
	{
		MapSize = FVector2Int(2 + StageLevel, 2 + StageLevel);
		DFS->StartAlgorithm(MapSize);
		Board = DFS->GetBoard();
		StartPos = DFS->GetStartPostion();
		StartCell = DFS->GetStartCell();
		PlayerCurrentCell = StartCell;
		BossCell = DFS->GetBossCell();
		BossPrevCell = DFS->GetBossPrevCell();
		TotalCellNum = DFS->GetTotalCellNum();
	}
	ClearCount = 0;
	bIsEarlyDiscoveredBoss = false;
}

void URLGameInstance::GenerateTutorialMap()
{
	if (DFS.IsValid())
	{
		MapSize = FVector2Int(2, 2);
		DFS->GenerateTutorialMap();

		Board = DFS->GetBoard();
		StartCell = DFS->GetStartCell();
		PlayerCurrentCell = StartCell;
		BossCell = DFS->GetBossCell();
		BossPrevCell = DFS->GetBossPrevCell();
		TotalCellNum = DFS->GetTotalCellNum();
	}
}

void URLGameInstance::SaveGame(UPARAM(ref) FTransform& PlayerTransform)
{
	URoguelikeSaveGame* SaveGame = Cast<URoguelikeSaveGame>(UGameplayStatics::CreateSaveGameObject(URoguelikeSaveGame::StaticClass()));
	check(SaveGame);
	if (SetTempManageDelegate.IsBound()) SetTempManageDelegate.Broadcast(); //Set Temp variables 
	FInfoRecord InfoRecord{TempFixMaxNum, Board, MapSize, ClearCount, StartPos, StartCell, PlayerCurrentCell, StageLevel, BossCell, BossPrevCell, TotalCellNum, bIsEarlyDiscoveredBoss, TempHealthManager, TempCombatManager, TempItemManager, TempBuff, PlayerTransform, TempDashChargeNum };

	SaveGame->SaveInfoRecord(InfoRecord);
	SaveGame->SaveItemInfos(TempItemInfos);
}

void URLGameInstance::LoadGame()
{
	LoadState = ELoadState::LoadGame;
	URoguelikeSaveGame* SaveGame = Cast<URoguelikeSaveGame>(UGameplayStatics::CreateSaveGameObject(URoguelikeSaveGame::StaticClass()));
	check(SaveGame);
	
	FInfoRecord InfoRecord;
	SaveGame->LoadInfoRecord(InfoRecord);
	LoadDataSetting(InfoRecord);
	
	TempItemInfos = SaveGame->LoadItemInfos();
}

void URLGameInstance::LoadDataSetting(const FInfoRecord& Record)
{
	Board = Record.Board;
	MapSize = Record.MapSize;
	ClearCount = Record.ClearCount;
	StartPos = Record.StartPos;
	StartCell = Record.StartCell;
	PlayerCurrentCell = Record.PlayerCurrentCell;
	StageLevel = Record.StageLevel;
	BossCell = Record.BossCell;
	BossPrevCell = Record.BossPrevCell;
	TotalCellNum = Record.TotalCellNum;
	bIsEarlyDiscoveredBoss = Record.bIsEarlyDiscoveredBoss;
	TempFixMaxNum = Record.FixMaxNum;
	TempHealthManager = Record.HealthManager;
	TempCombatManager = Record.CombatManager;
	TempItemManager = Record.ItemManager;
	TempBuff = Record.Buff;
	TempPlayerTransform = Record.PlayerTransform;
	TempDashChargeNum = Record.TempDashChargeNum;
}

void URLGameInstance::TestPrintMap()
{
	for (int32 i = 0; i < Board.Num(); ++i)
	{
		for (int32 j = 0; j < Board[i].Status.Num(); ++j)
		{
			UE_LOG(LogTemp, Warning, TEXT("Board %d : Status - %d"), i, Board[i].Status[j]);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CurrentCell : %d"), PlayerCurrentCell);
	UE_LOG(LogTemp, Warning, TEXT("StartCell : %d"), StartCell);
	UE_LOG(LogTemp, Warning, TEXT("BossCell : %d"), BossCell);
	UE_LOG(LogTemp, Warning, TEXT("BossPrevCell : %d"), BossPrevCell);
	UE_LOG(LogTemp, Warning, TEXT("TotalCellNum : %d"), TotalCellNum);
	UE_LOG(LogTemp, Warning, TEXT("ClearCount : %d"), ClearCount);
	UE_LOG(LogTemp, Warning, TEXT("TotalCellNum - 1 == ClearCount -> Can Move Boss"));


	//RequestMoveNextStage();
	//ClearStage();
}

URLGameInstance* URLGameInstance::GetRLGameInst(const UObject* WorldContextObject)
{
	return Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

void URLGameInstance::RequestInfo() const
{
	if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->SetMapInfo(MapSize, Board, PlayerCurrentCell);
	}
}

TArray<uint8> URLGameInstance::GetConnectedDir()
{
	TArray<uint8> Ret;
	if (PlayerCurrentCell - MapSize.Y >= 0) //위에 셀이 있다.
	{
		if (Board[PlayerCurrentCell].Status[0] && Board[PlayerCurrentCell - MapSize.Y].Status[1])
		{
			Ret.Add(0);
		}
	}
	if (PlayerCurrentCell + MapSize.Y < Board.Num()) //아래 셀이 있다.
	{
		if (Board[PlayerCurrentCell].Status[1] && Board[PlayerCurrentCell + MapSize.Y].Status[0])
		{
			Ret.Add(1);
		}
	}
	if ((PlayerCurrentCell + 1) % MapSize.Y != 0) //오른쪽 셀이 있다
	{
		if (Board[PlayerCurrentCell].Status[2] && Board[PlayerCurrentCell + 1].Status[3])
		{
			Ret.Add(2);
		}
	}
	if (PlayerCurrentCell % MapSize.X != 0) //왼쪽 셀이 있다.
	{
		if (Board[PlayerCurrentCell].Status[3] && Board[PlayerCurrentCell - 1].Status[2])
		{
			Ret.Add(3);
		}
	}

	return Ret;
}

void URLGameInstance::RequestMove(uint8 Dir) //0,1,2,3중 하나로 넘어옴
{
	uint8 NextCell = CalcNextCell(Dir);
	if (NextCell == BossCell)
	{
		if (TotalCellNum - 1 != ClearCount) //반대편 셀을 모두 클리어하지 않았으면
		{
			if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
			{
				Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->ShowNoticeWidget(TEXT("다른 방을 클리어해야 해요."));
			}
			return;
		}
	}
	else if (NextCell == BossPrevCell)
	{
		if (TotalCellNum - 2 > ClearCount) //조기 발견
		{
			bIsEarlyDiscoveredBoss = true;
		}
		Board[BossCell].CellState = ECellState::DiscoveredBoss;
	}

	MoveProcess(NextCell, Dir);
	if (PlayerCurrentCell != BossCell)
	{
		CheckEarlyDiscoveredBossCell();
	}
}

void URLGameInstance::RequestMovePrevBossCell()
{
	MoveProcess(BossPrevCell, -1);
}

int32 URLGameInstance::CalcNextCell(uint8 Dir) const
{
	switch (Dir)
	{
		case 0:
			return PlayerCurrentCell - MapSize.Y;
		case 1:
			return PlayerCurrentCell + MapSize.Y;
		case 2:
			return PlayerCurrentCell + 1;
		case 3:
			return PlayerCurrentCell - 1;
	}
	return 0;
}

void URLGameInstance::ClearThisCell()
{
	if (!Board[PlayerCurrentCell].IsCleared)
	{
		Board[PlayerCurrentCell].IsCleared = true;
		ClearCount++;
		CheckEarlyDiscoveredBossCell();
	}
}

void URLGameInstance::CheckEarlyDiscoveredBossCell()
{
	if ((TotalCellNum - 1 == ClearCount) && bIsEarlyDiscoveredBoss)
	{
		RLGameState = RLGameState ? RLGameState : Cast<ARLMainGameState>(UGameplayStatics::GetGameState(this));
		if (RLGameState)
		{
			RLGameState->SpawnPrevBossPortal();
		}
	}
}

void URLGameInstance::AteHealThisCell()
{
	Board[PlayerCurrentCell].IsCleared = true;
}

void URLGameInstance::RequestMoveNextStage()
{
	StageLevel++;
	GenerateMap();
	
	InitOnceItemDelegate.ExecuteIfBound();
	MoveProcess(StartCell, -1);
}

void URLGameInstance::ReadyToRecall()
{
	if (Board[PlayerCurrentCell].IsCleared)
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RecallStart();
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RecallCompleteSuccessfully.BindUObject(this, &ThisClass::Recall);
	}
}

void URLGameInstance::Recall()
{
	MoveProcess(StartCell, -1);
}

void URLGameInstance::MoveProcess(int32 TargetCell, uint8 Dir)
{
	Board[PlayerCurrentCell].CellState = ECellState::Clear;
	if ((Board[PlayerCurrentCell].CellType == ECellType::Bonus) && !Board[PlayerCurrentCell].IsCleared)
	{
		Board[PlayerCurrentCell].CellState = ECellState::Bonus;
	}
	PlayerCurrentCell = TargetCell;
	Board[PlayerCurrentCell].CellState = ECellState::InPlayer;

	if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RemoveMinimapWidget();
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->SetMapInfo(MapSize, Board, PlayerCurrentCell);
	}
	
	if (LoadState == ELoadState::Tutorial)
	{
		ARLTutorialGameMode* RLTutoGM = Cast<ARLTutorialGameMode>(UGameplayStatics::GetGameMode(this));
		if (RLTutoGM && RLTutoGM->GetTutorialManager())
		{
			RLTutoGM->GetTutorialManager()->ReconstructCell(Dir, Board[PlayerCurrentCell]);
		}
	}
	else
	{
		RLGameState = RLGameState ? RLGameState : Cast<ARLMainGameState>(UGameplayStatics::GetGameState(this));
		if (RLGameState)
		{
			RLGameState->ReconstructCell(Dir, StageLevel, Board[PlayerCurrentCell]);
		}
	}
}

URLListenerManager* URLGameInstance::GetListenerManager() const
{
	return ListenerManager;
}

bool URLGameInstance::CanSaveThisCell()
{
	bool HaveAnyState = false;
	if (ListenerManager)
	{
		HaveAnyState = ListenerManager->HaveCCState();
		if (HaveAnyState) return false;
	}
	if (StageLevel == 0) return false;
	if (Board.IsValidIndex(PlayerCurrentCell))
	{
		return (Board[PlayerCurrentCell].IsCleared) || (Board[PlayerCurrentCell].CellType == ECellType::Bonus);
	}
	return false;
}

void URLGameInstance::GetManager(OUT FHealthManager& OutHealthManager, OUT FCombatManager& OutCombatManager, OUT uint8& OutBuff) const
{ 
	OutHealthManager = TempHealthManager;
	OutCombatManager = TempCombatManager;
	OutBuff = TempBuff;
}
void URLGameInstance::GetManager(OUT FItemManager& OutItemManager, OUT TMap<uint8, uint8>& OutFixMaxNum, OUT TArray<UItemInfo*>& OutItemInfos) const
{
	OutItemManager = TempItemManager;
	OutFixMaxNum = TempFixMaxNum;
	OutItemInfos = TempItemInfos;
}
void URLGameInstance::SetManager(const FHealthManager& InHealthManager, const FCombatManager& InCombatManager, const uint8& InBuff)
{
	TempHealthManager = InHealthManager;
	TempCombatManager = InCombatManager;
	TempBuff = InBuff;
}
void URLGameInstance::SetManager(const FItemManager& InItemManager, const TMap<uint8, uint8>& InFixMaxNum, const TArray<UItemInfo*>& InSaveItemData)
{
	TempItemManager = InItemManager;
	TempFixMaxNum = InFixMaxNum;
	TempItemInfos = InSaveItemData;
}
void URLGameInstance::SetSelectedBonusItem(bool Boolean)
{
	if (Board.IsValidIndex(PlayerCurrentCell))
	{
		Board[PlayerCurrentCell].SelectedBonusItem = Boolean;
	}
}
FCell URLGameInstance::GetCellInfo() const 
{ 
	if (Board.IsValidIndex(PlayerCurrentCell))
	{
		return Board[PlayerCurrentCell];
	}
	return FCell();
}

bool URLGameInstance::CanBind(FName MappingName, FKey Key)
{
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	check(Settings);
	const TArray<FInputAxisKeyMapping>& Axi = Settings->GetAxisMappings();
	const TArray<FInputActionKeyMapping>& Actions = Settings->GetActionMappings();

	for (auto Axis : Axi)
	{
		if (Axis.Key.GetFName() == Key.GetFName())
		{
			if (Axis.AxisName == MappingName)
			{
				return false;
			}
		}
	}
	for (auto Action : Actions)
	{
		if (Action.Key.GetFName() == Key.GetFName())
		{
			if (Action.ActionName == MappingName)
			{
				return false;
			}
		}
	}
	return true;
}

int32 URLGameInstance::GetMappingNum(FName MappingName)
{
	UInputSettings* Settings = UInputSettings::GetInputSettings();
	check(Settings);
	if (Settings->DoesAxisExist(MappingName))
	{
		TArray<FInputAxisKeyMapping> Mappings;
		Settings->GetAxisMappingByName(MappingName, Mappings);
		return Mappings.Num();
	}
	return 0;
}

void URLGameInstance::RemoveBindKey(FKey Key) // W 가 들어왔으면 W랑 매핑된 걸 찾는다.
{
	UInputSettings* Settings = UInputSettings::GetInputSettings();
	check(Settings);
	
	TArray<FInputActionKeyMapping> Actions = Settings->GetActionMappings();
	TArray<FInputAxisKeyMapping> Axes = Settings->GetAxisMappings();

	//W는 축매핑. 축 매핑이면 그게 또 -1인지 1인지 알아서 -1인거면 -1인걸 None으로 해줘야한다.
	
	for (int32 i = 0; i < Actions.Num(); ++i)
	{
		if (Actions[i].Key.GetFName() == Key.GetFName())
		{
			Settings->RemoveActionMapping(Actions[i]);
			FInputActionKeyMapping NewMapping(Actions[i].ActionName, FKey());
			Settings->AddActionMapping(NewMapping);
			break;
		}
	}

	for (int32 i = 0; i < Axes.Num(); ++i)
	{
		if (Axes[i].Key.GetFName() == Key.GetFName()) //S랑 같은걸 찾음.
		{
			float AxisValue = Axes[i].Scale; //-1 기억
			
			Settings->RemoveAxisMapping(Axes[i]); //S 매핑 삭제
			FInputAxisKeyMapping NewMapping(Axes[i].AxisName, FKey()); //S 매핑되어있던 이름에 키를 None으로 생성
			NewMapping.Scale = AxisValue; //기억해두었던 Scale 삽입
			Settings->AddAxisMapping(NewMapping); //새로운 매핑 추가. 즉, 같은 매핑인데 키만 None.

			break;
		}
	}
}

void URLGameInstance::ChangeBindKey(FName MappingName, FKey Key, float ValueIfAxis)
{
	UInputSettings* Settings = UInputSettings::GetInputSettings();
	check(Settings);

	if (Settings->DoesActionExist(MappingName))
	{
		TArray<FInputActionKeyMapping> Mappings;
		Settings->GetActionMappingByName(MappingName, Mappings);
		for (auto& Mapping : Mappings)
		{
			Settings->RemoveActionMapping(Mapping);
			FInputActionKeyMapping NewMapping(MappingName, Key);
			Settings->AddActionMapping(NewMapping);
		}
	}
	else if (Settings->DoesAxisExist(MappingName)) 
	{											
		TArray<FInputAxisKeyMapping> Mappings;
		Settings->GetAxisMappingByName(MappingName, Mappings);
		for (auto& Mapping : Mappings) 
		{
			if (Mapping.Scale == ValueIfAxis)
			{
				FInputAxisKeyMapping OriginalMapping = Mapping;
				OriginalMapping.Key = Key;
				
				Settings->RemoveAxisMapping(Mapping);
				Settings->AddAxisMapping(OriginalMapping);
				
				break;
			}
		}
	}
}

void URLGameInstance::RollbackBindKey(FName MappingName, FKey Key, float ValueIfAxis)
{                                 
	if(!CanBind(MappingName, Key)) return;

	UInputSettings* Settings = UInputSettings::GetInputSettings();
    check(Settings);

	if (Settings->DoesActionExist(MappingName))
	{
		TArray<FInputActionKeyMapping> Mappings;
		Settings->GetActionMappingByName(MappingName, Mappings);
		for (auto& Mapping : Mappings)
		{
			Settings->RemoveActionMapping(Mapping);
			FInputActionKeyMapping NewMapping(MappingName, Key);
			Settings->AddActionMapping(NewMapping);
		}
	}
    else if (Settings->DoesAxisExist(MappingName))
    {
    	AxisRollbackCount++;
    	TArray<FInputAxisKeyMapping> Mappings;
    	Settings->GetAxisMappingByName(MappingName, Mappings);

    	FInputAxisKeyMapping OriginalMapping;

    	bool IsDoubleNone = true;
    	for (auto& Mapping : Mappings)
    	{
			if (Mapping.Key != FKey())
			{
				IsDoubleNone = false;
			}
    	}
		if(!IsDoubleNone)
		{
			for (auto& Mapping : Mappings) 
			{
				if (Mapping.Scale == ValueIfAxis)
				{
					OriginalMapping = Mapping;
					OriginalMapping.Key = Key;
					OriginalMapping.Scale = ValueIfAxis;
    			
					Settings->AddAxisMapping(OriginalMapping);
					Settings->RemoveAxisMapping(Mapping);
				}
			}
		}
		else
		{
			for (auto& Mapping : Mappings) 
			{
				if (Mapping.Scale == ValueIfAxis)
				{
					OriginalMapping = Mapping;
					OriginalMapping.Key = Key;
					OriginalMapping.Scale = ValueIfAxis;
    			
					Settings->AddAxisMapping(OriginalMapping);
				}
			}
			if (AxisRollbackCount == 2)
			{
				RemoveNoneAxis(MappingName);
				AxisRollbackCount = 0;
			}
		}
    }
}

void URLGameInstance::RemoveNoneAxis(FName MappingName)
{
	UInputSettings* Settings = UInputSettings::GetInputSettings();
	check(Settings);

	if (Settings->DoesAxisExist(MappingName))
	{
		TArray<FInputAxisKeyMapping> Mappings;
		Settings->GetAxisMappingByName(MappingName, Mappings);
    	
		for (auto& Mapping : Mappings) 
		{
			if (Mapping.Key == FKey())
			{
				Settings->RemoveAxisMapping(Mapping);
			}
		}
	}
}
