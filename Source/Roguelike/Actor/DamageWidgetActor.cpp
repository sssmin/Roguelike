// Fill out your copyright notice in the Description page of Project Settings.
#include "DamageWidgetActor.h"

#include "Components/WidgetComponent.h"
#include "Roguelike/Widget/DamageWidget.h"

ADamageWidgetActor::ADamageWidgetActor()
{
	PrimaryActorTick.bCanEverTick = false;

	NumWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComp"));
	RootComponent = NumWidgetComp;
	DestroyTime = 1.5f;
}

void ADamageWidgetActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyThisActor, DestroyTime, false);
}

void ADamageWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageWidgetActor::DestroyThisActor()
{
	Destroy();
}

void ADamageWidgetActor::PlayNumWidget(bool IsPlayer, bool IsCritical, float Damage, bool IsHeal, bool IsDodge)
{
	if (NumWidgetComp)
	{
		UUserWidget* Widget = NumWidgetComp->GetWidget();
		if (Widget)
		{
			UDamageWidget* DamageWidget = Cast<UDamageWidget>(Widget);
			if (DamageWidget)
			{
				DamageWidget->Play(IsPlayer, IsCritical, Damage, IsHeal, IsDodge);
			}
		}
	}
}

