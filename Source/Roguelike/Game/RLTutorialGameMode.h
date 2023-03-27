// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "RLBaseGameMode.h"
#include "RLTutorialGameMode.generated.h"

class UTutorialWidget;
class UFadeWidget;
class UTutorialManager;
struct FCell;

UCLASS()
class ROGUELIKE_API ARLTutorialGameMode : public ARLBaseGameMode
{
	GENERATED_BODY()

public:
	ARLTutorialGameMode();
	virtual void BeginPlay() override;
	void SpawnTutorialMonster();
	UTutorialManager* GetTutorialManager() const;
	void CreateTutorialPortal(uint8 Dir) const;
	
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UTutorialWidget> TutorialWidgetClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFadeWidget> FadeWidgetClass;
	UPROPERTY()
	UTutorialManager* TutorialManager;

	UPROPERTY()
	UTutorialWidget* TutorialWidget;

public:
	UTutorialWidget* GetTutorialWidget() const { return TutorialWidget; }
	TSubclassOf<UFadeWidget> GetFadeWidgetClass() const { return FadeWidgetClass; }
	
};
