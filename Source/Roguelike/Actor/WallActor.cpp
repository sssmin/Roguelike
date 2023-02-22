// Fill out your copyright notice in the Description page of Project Settings.
#include "WallActor.h"

#include "Roguelike/Roguelike.h"

AWallActor::AWallActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMComp"));
	RootComponent = SMComp;

	SMComp->SetCollisionObjectType(ECC_WallBlockProjectile);
	SMComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);
}

void AWallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWallActor::SetSM(int32 CellClass)
{
	if (SMComp && !Walls.IsEmpty())
	{
		if (CellClass == 2 || CellClass == 3)
		{
			SMComp->SetStaticMesh(Walls[0]);
		}
		else if (CellClass == 4)
		{
			SMComp->SetStaticMesh(Walls[1]);
		}
	}
}
