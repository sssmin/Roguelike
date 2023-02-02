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
	for (int32 i = 0; i < Portals.Num(); ++i)
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), Actors);
		
		for (int32 j = 0; j < Actors.Num(); ++j)
		{
			ATargetPoint* TP = Cast<ATargetPoint>(Actors[j]);
			if (TP)
			{
				TArray<FName> TPTag = TP->Tags;
				if (TPTag.IsValidIndex(0))
				{
					FString Str = TPTag[0].ToString();
					int32 StrToInt = FCString::Atoi(*Str);
					if (StrToInt == Portals[i].Dir)
					{
						Portals[i].Portal->SetActorLocation(TP->GetActorLocation());
					}
				}
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