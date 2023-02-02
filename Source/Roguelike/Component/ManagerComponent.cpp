// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerComponent.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

UManagerComponent::UManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	
}


void UManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	
	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && Cast<APlayerCharacter>(GetOwner()))
	{
		CurrentManager = GI->GetManager();
		GI->OnMoveMap.BindUObject(this, &ThisClass::SendManager);
	}
}


void UManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UManagerComponent::SendManager()
{
	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		GI->SetManager(CurrentManager);
	}
}

