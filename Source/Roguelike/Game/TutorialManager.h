// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Roguelike/Interface/DFSInterface.h"
#include "TutorialManager.generated.h"

class ARLTutorialGameMode;
class URLGameInstance;

UENUM(BlueprintType)
enum class ETutorialTypes : uint8
{
	Desc,
	ActivePortal,
	SpawnMob,
	ItemWidget
};

USTRUCT(BlueprintType)
struct FTutorialStep : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TypeStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETutorialTypes TutorialTypes;
};

USTRUCT(BlueprintType)
struct FTutorialDesc : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Step;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Desc;
};

UCLASS()
class UTutorialSteps : public UObject
{
	GENERATED_BODY()
	
public:
	int32 TypeStep;
	ETutorialTypes TutorialTypes;

	static UTutorialSteps* ConstructSteps(int32 TypeStep, ETutorialTypes TutorialTypes)
	{
		UTutorialSteps* Steps = NewObject<UTutorialSteps>();
		Steps->Init(TypeStep, TutorialTypes);
		return Steps;
	}
private:
	void Init(int32 InTypeStep, ETutorialTypes InTutorialTypes)
	{
		TypeStep = InTypeStep;
		TutorialTypes = InTutorialTypes;
	}
};

UCLASS()
class UTutorialDescs : public UObject
{
	GENERATED_BODY()
	
public:
	int32 Step;
	FString Desc;
	
	static UTutorialDescs* ConstructDescs(int32 InStep, FString InDesc)
	{
		UTutorialDescs* Descs = NewObject<UTutorialDescs>();
		Descs->Init(InStep, InDesc);
		return Descs;
	}
private:
	void Init(int32 InStep, FString InDesc)
	{
		Step = InStep;
		Desc = InDesc;
	}
};


UCLASS()
class ROGUELIKE_API UTutorialManager : public UObject
{
	GENERATED_BODY()

public:
	UTutorialManager();
	virtual void PostInitProperties() override;
	void SetObjective();
	void ReconstructCell(uint8 Dir, const FCell& Info);
	void SetNextTutorial();
	void TutorialCompleted();
	
private:
	void StartTutorial();
	void InitTutorialTable();
	void InitDescTable();
	void EndTutorial();
	
	void ExecuteTutorial();
	void DescTypingSkip();
	bool CheckTypeCompleted();
	void VisibleDesc(bool Visible);
	uint8 GetDir();
	uint8 CalcConnectedDir(TArray<uint8> Dirs, uint8 SideDir);
	
	UPROPERTY()
	URLGameInstance* GameInst;
	UPROPERTY()
	ARLTutorialGameMode* TutorialGameMode;
	FCell CellInfo;
	UPROPERTY()
	TArray<UTutorialSteps*> Tutorials;
	UPROPERTY()
	TArray<UTutorialDescs*> Descs;
	int32 CurrentStep; //Tutorials Index
	
	uint8 PrevEnterDir;
	
	
public:
	void SetCellInfo(const FCell& InInfo) { CellInfo = InInfo; }
	
};
