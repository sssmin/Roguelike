// Fill out your copyright notice in the Description page of Project Settings.


#include "CellActor.h"
#include "Roguelike/Component/PortalComponent.h"
#include "Components/ArrowComponent.h"
#include "WallActor.h"

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
	if (!Walls.IsEmpty())
	{
		for (auto WallActor : Walls)
		{
			WallActor->Destroy();
		}
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

void ACellActor::CreateWall()
{
	TArray<UArrowComponent*> ArrowComponents;
	GetComponents(ArrowComponents);
	TMap<int32, FVector> SpawnLocation;
	TMap<int32, FRotator> SpawnRotation;

	for (auto Arrow : ArrowComponents)
	{
		UArrowComponent* ArrowComp = Cast<UArrowComponent>(Arrow);
		if (ArrowComp)
		{
			TArray<FName> TPTag = ArrowComp->ComponentTags;
			if (TPTag.IsValidIndex(1))
			{
				FString Str = TPTag[1].ToString();
				int32 StrToInt = FCString::Atoi(*Str);
				SpawnLocation.Add(StrToInt, ArrowComp->GetComponentLocation());
				SpawnRotation.Add(StrToInt, ArrowComp->GetComponentRotation());
			}
		}
	}
	for (int32 i = 0; i < SpawnLocation.Num(); ++i)
	{
		if (IsExistWall(i))
		{
			if (WallActorClass)
			{
				FTransform SpawnTransform{ SpawnRotation[i], SpawnLocation[i], FVector(2.f, 2.f, 2.f) };
				AWallActor* WallActor = GetWorld()->SpawnActor<AWallActor>(WallActorClass, SpawnTransform);
				if (WallActor)
				{
					Walls.Add(WallActor);
					WallActor->SetSM(CellClass);
				}
				
			}
			
			
		}
	}

}

bool ACellActor::IsExistWall(uint8 Wall)
{
	UE_LOG(LogTemp, Warning, TEXT("IsExistWall Func : %d"), Wall);
	UE_LOG(LogTemp, Warning, TEXT("IsExistWall Func TempWall : %d"), TempWall);
	return TempWall & 1 << Wall;
}