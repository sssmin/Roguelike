// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomActor.h"

// Sets default values
ARoomActor::ARoomActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = RootComp;

	for (int32 i = 0; i < 4; ++i)
	{
		FString Text = FString::Printf(L"Wall%d", i);
		FName T = FName(*Text);
		Walls.Add(CreateDefaultSubobject<UStaticMeshComponent>(T));
		Walls[i]->SetupAttachment(RootComponent);
		Text = FString::Printf(L"Door%d", i);
		T = FName(*Text);
		Doors.Add(CreateDefaultSubobject<UStaticMeshComponent>(T));
		Doors[i]->SetupAttachment(RootComponent);
	}
}

void ARoomActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomActor::UpdateRoom(TArray<bool> Status)
{
	for (int i = 0; i < Status.Num() -1; ++i)
	{
		if (Status[i])
		{
			//Doors[i]->GetChildComponent(0)->Activate();
			Walls[i]->GetChildComponent(0)->DestroyComponent();
			//Doors[i]->Activate();
			//Walls[i]->Deactivate();
		}
		else
		{
			//Doors[i]->Deactivate();
			//Walls[i]->Activate();
			Doors[i]->GetChildComponent(0)->DestroyComponent();
			//Walls[i]->GetChildComponent(0)->Activate();
		}
	}
}



