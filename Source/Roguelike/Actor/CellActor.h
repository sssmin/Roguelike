// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Roguelike/Interface/DFSInterface.h"
#include "CellActor.generated.h"

class UPortalComponent;
class AWallActor;

UCLASS()
class ROGUELIKE_API ACellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACellActor();
	virtual void BeginPlay() override;
	void CreateSidePortal();
	void CreateCenterPortal();
	void CreatePrevBossPortal();
	void CreateTutorialPortal(uint8 Dir);
	FVector GetPlayerSpawnLocation(uint8 Dir);
	void CreateWall();
	UFUNCTION(BlueprintImplementableEvent)
	FVector GetPlaneScale();
	
protected:
	virtual void Destroyed() override;

private:
	bool IsExistWall(uint8 Wall);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = ETempWall, AllowPrivateAccess = "true"))
	uint8 TempWall;
	UPROPERTY()
	int32 CellClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UPortalComponent* PortalComp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWallActor> WallActorClass;
	UPROPERTY()
	TArray<AWallActor*> Walls;
	
public:
	void SetTempWall(int32 CellIndex, uint8 Value) 
	{ 
		TempWall = Value; 
		CellClass = CellIndex;
	}
};
