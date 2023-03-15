// Fill out your copyright notice in the Description page of Project Settings.
#include "PortalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"

#include "Roguelike/Actor/PortalActor.h"
#include "Roguelike/Game/RLGameInstance.h"


UPortalComponent::UPortalComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UPortalComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UPortalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPortalComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UPortalComponent::CreatePortal(TArray<uint8> Dirs)
{
	if (GetWorld() && PortalActorClass)
	{
		for (int32 i = 0; i < Dirs.Num(); ++i)
		{
			APortalActor* Portal = Cast<APortalActor>(GetWorld()->SpawnActor(PortalActorClass));
			if (Portal)
			{
				Portal->SetDir(Dirs[i]);
				Portals.Add(FPortalInfo(Dirs[i], Portal));
			}
		}
		SetLocationPotal();
	}
}

void UPortalComponent::SetLocationPotal()
{
	TArray<UArrowComponent*> ArrowComponents;
	GetOwner()->GetComponents(ArrowComponents);

	TMap<int32, FVector> SpawnLocation;

	for (auto Arrow : ArrowComponents)
	{
		UArrowComponent* ArrowComp = Cast<UArrowComponent>(Arrow);
		if (ArrowComp)
		{
			TArray<FName> TPTag = ArrowComp->ComponentTags;
			if (TPTag.IsValidIndex(0))
			{
				FString Str = TPTag[0].ToString();
				int32 StrToInt = FCString::Atoi(*Str);
				SpawnLocation.Add(StrToInt, ArrowComp->GetComponentLocation());
			}
		}
	}

	for (auto PortalInfo : Portals)
	{
		if (SpawnLocation.Find(PortalInfo.Dir))
		{
			PortalInfo.Portal->SetActorLocation(*SpawnLocation.Find(PortalInfo.Dir));
		}
	}
}

void UPortalComponent::CreateSidePortal()
{
	if (GetWorld())
	{
		URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
		if (GI)
		{
			CreatePortal(GI->GetConnectedDir());
		}
	}
	
	for (auto PortalInfo : Portals)
	{
		if (PortalInfo.Portal)
		{
			PortalInfo.Portal->SetSidePortal();
		}
	}
}

void UPortalComponent::CreateCenterPortal()
{
	if (GetWorld())
	{
		APortalActor* Portal = Cast<APortalActor>(GetWorld()->SpawnActor(PortalActorClass));
		if (Portal)
		{
			Portal->SetCenterPortal();
			Portals.Add(FPortalInfo(-1, Portal));
		}
	}
}

void UPortalComponent::CreatePrevBossPortal()
{
	if (GetWorld())
	{
		PrevBossPortal = Cast<APortalActor>(GetWorld()->SpawnActor(PortalActorClass));
		if (PrevBossPortal)
		{
			PrevBossPortal->SetPrevBossPortal();
			
		}
	}
}

void UPortalComponent::DestroyPortal()
{
	for (int32 i = 0; i < Portals.Num(); ++i)
	{
		Portals[i].Portal->Destroy();
	}
	Portals.Empty();
	if (PrevBossPortal)
	{
		PrevBossPortal->Destroy();
		PrevBossPortal = nullptr;
	}
}

FVector UPortalComponent::GetArrowLocation(uint8 Dir)
{
	FVector SpawnLocation;
	switch (Dir)
	{
	case 0:
		Dir = 1;
		SpawnLocation = CalcLocation(Dir) + FVector(200.f, 0.f, 0.f);
		break;
	case 1:
		Dir = 0;
		SpawnLocation = CalcLocation(Dir) + FVector(-200.f, 0.f, 0.f);
		break;
	case 2:
		Dir = 3;
		SpawnLocation = CalcLocation(Dir) + FVector(0.f, 200.f, 0.f);
		break;
	case 3:
		Dir = 2;
		SpawnLocation = CalcLocation(Dir) + FVector(0.f, -200.f, 0.f);
		break;
	}

	return SpawnLocation;
}

FVector UPortalComponent::CalcLocation(uint8 Dir)
{
	TArray<UArrowComponent*> ArrowComponents;
	GetOwner()->GetComponents(ArrowComponents);

	for (auto Arrow : ArrowComponents)
	{
		UArrowComponent* ArrowComp = Cast<UArrowComponent>(Arrow);
		if (ArrowComp)
		{
			TArray<FName> TPTag = ArrowComp->ComponentTags;
			if (TPTag.IsValidIndex(0))
			{
				FString Str = TPTag[0].ToString();
				uint8 StrToInt = FCString::Atoi(*Str);
				if (Dir == StrToInt)
				{
					return ArrowComp->GetComponentLocation();
				}
			}
		}
	}
	return FVector::ZeroVector;
}