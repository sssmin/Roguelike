// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerComponent.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

UManagerComponent::UManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HealthManager = FHealthManage();
	CombatManager = FCombatManage();
	CurrentState = 0;
}


void UManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	
	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && Cast<APlayerCharacter>(GetOwner()))
	{
		GI->GetManager(HealthManager, CombatManager, CurrentState);
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
		GI->SetManager(HealthManager, CombatManager, CurrentState);
	}
}

void UManagerComponent::ReceiveDamage(const FCombatManage& EnemyCombatManage)
{
	if (CheckState(EState::DEAD)) return;
	//상성 계산
	const float Coefficient = CalcCounter(EnemyCombatManage.Element);
	const float EnemyATK = EnemyCombatManage.ATK;
	//대미지 계산
	const float Result = FMath::Clamp((EnemyATK * Coefficient) * FMath::RandRange(0.9, 1.1), 0.f, TNumericLimits<int32>::Max());
	HealthManager.CurrentHP = FMath::Clamp(HealthManager.CurrentHP - Result, 0.f, TNumericLimits<int32>::Max());

	if (HealthManager.CurrentHP <= 0.f)
	{
		GiveState(EState::DEAD);
	}
}

void UManagerComponent::GiveState(EState State)
{
	CurrentState |= static_cast<uint8>(State);
}

void UManagerComponent::RemoveState(EState State)
{
	CurrentState ^= static_cast<uint8>(State);
}

bool UManagerComponent::CheckState(EState State)
{
	return CurrentState & static_cast<uint8>(State);
}

float UManagerComponent::CalcCounter(EElement EnemyElement)//상성. 받을 대미지 계수
{
	float Ret = 1.f;

	switch (CombatManager.Element)
	{
	case EElement::NONE:
		Ret = 1.f;
		break;
	case EElement::FIRE:
		if (EnemyElement == EElement::WATER)
		{
			Ret = 2.f;
		}
		else if (EnemyElement == EElement::EARTH)
		{
			Ret = 0.5f;
		}
		break;
	case EElement::WATER:
		if (EnemyElement == EElement::EARTH)
		{
			Ret = 2.f;
		}
		else if (EnemyElement == EElement::FIRE)
		{
			Ret = 0.5f;
		}
		break;
	case EElement::EARTH:
		if (EnemyElement == EElement::FIRE)
		{
			Ret = 2.f;
		}
		else if (EnemyElement == EElement::WATER)
		{
			Ret = 0.5f;
		}
		break;
	case EElement::DARKNESS:
		if (EnemyElement == EElement::LIGHT)
		{
			Ret = 2.f;
		}
		break;
	case EElement::LIGHT:
		if (EnemyElement == EElement::DARKNESS)
		{
			Ret = 2.f;
		}
		break;
	}
	return Ret;
}