// Fill out your copyright notice in the Description page of Project Settings.


#include "CellActor.h"
#include "Roguelike/Component/PortalComponent.h"

ACellActor::ACellActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PortalComp = CreateDefaultSubobject<UPortalComponent>(TEXT("PortalComponent"));
}

void ACellActor::BeginPlay()
{
	Super::BeginPlay();

}

void ACellActor::Destroyed()
{
	Super::Destroyed();

	if (PortalComp)
	{
		PortalComp->DestroyPortal();
	}
}

void ACellActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACellActor::CreateSidePortal()
{
	if (PortalComp)
	{
		PortalComp->CreateSidePortal();
	}
}

void ACellActor::CreateCenterPortal()
{
	if (PortalComp)
	{
		PortalComp->CreateCenterPortal();
	}
}

FVector ACellActor::GetPlayerSpawnLocation(int32 Dir)
{
	if (PortalComp)
	{
		return PortalComp->GetArrowLocation(Dir);
	}
	return FVector::ZeroVector;
}

