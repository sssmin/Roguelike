// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalComponent.h"
#include "Components/SphereComponent.h"
#include "Roguelike/PortalActor.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

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

	URLGameInstance* RLGameInstance = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (RLGameInstance)
	{
		CreatePortal(RLGameInstance->GetConnectedDir());
	}
}

void UPortalComponent::CreatePortal(TArray<int32> Dirs)
{
	if (GetWorld() && PortalActorClass)
	{
		for (int32 i = 0; i < Dirs.Num(); ++i)
		{
			FActorSpawnParameters Params;

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
	TArray<AActor*> TargetPointActors;
	UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointActors);
	
	TMap<int32, FVector> SpawnLocation;
	for (auto TagetPoint : TargetPointActors)
	{
		ATargetPoint* TP = Cast<ATargetPoint>(TagetPoint);
		if (TP)
		{
			TArray<FName> TPTag = TP->Tags;
			if (TPTag.IsValidIndex(0))
			{
				FString Str = TPTag[0].ToString();
				int32 StrToInt = FCString::Atoi(*Str);
				SpawnLocation.Add(StrToInt, TP->GetActorLocation());
			}
		}
	}
	
	
	for (auto PortalInfo : Portals)
	{
		if (SpawnLocation.Find(PortalInfo.Dir))
		{
			PortalInfo.Portal->SetActorLocation(*SpawnLocation.Find(PortalInfo.Dir));

			switch (PortalInfo.Dir)
			{
				case 0:
					if (SpawnLocation.Find(1))
						PortalInfo.Portal->SetOtherSide(*SpawnLocation.Find(1) + FVector(100.f, 0.f, 0.f));
					break;
				case 1:
					if (SpawnLocation.Find(0))
						PortalInfo.Portal->SetOtherSide(*SpawnLocation.Find(0) + FVector(-100.f, 0.f, 0.f));
					break;
				case 2:
					if (SpawnLocation.Find(3))
						PortalInfo.Portal->SetOtherSide(*SpawnLocation.Find(3) + FVector(0.f, 100.f, 0.f));
					break;
				case 3:
					if (SpawnLocation.Find(2))
						PortalInfo.Portal->SetOtherSide(*SpawnLocation.Find(2) + FVector(0.f, -100.f, 0.f));
					break;
			}
			
		}
		
	}
}

void UPortalComponent::ActiveAllPortal()
{
	for (auto PortalInfo : Portals)
	{
		if (PortalInfo.Portal)
		{
			PortalInfo.Portal->PortalParticleVisible(true);
			
		}
	}
}