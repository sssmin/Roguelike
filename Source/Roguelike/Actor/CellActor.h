// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Roguelike/Type/DFSInterface.h"
#include "CellActor.generated.h"

class UPortalComponent;
class AWallActor;

UCLASS()
class ROGUELIKE_API ACellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACellActor();
	virtual void Tick(float DeltaTime) override;
	void CreateSidePortal();
	void CreateCenterPortal();
	FVector GetPlayerSpawnLocation(int32 Dir);
	void CreateWall();
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = ETempWall, AllowPrivateAccess = "true"))
	uint8 TempWall;

	UPROPERTY()
	int32 CellClass;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UPortalComponent* PortalComp;


	bool IsExistWall(uint8 Wall);

	TArray<AWallActor*> Walls;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWallActor> WallActorClass;

public:
	void SetTempWall(int32 CellIndex, uint8 Value) 
	{ 
		TempWall = Value; 
		CellClass = CellIndex;
	}
};
