// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomActor.generated.h"


class UStaticMeshComponent;

UCLASS()
class ROGUELIKE_API ARoomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomActor();
	void UpdateRoom(TArray<bool> Status);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> Walls;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> Doors;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;
	
};
