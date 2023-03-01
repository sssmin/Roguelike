// Fill out your copyright notice in the Description page of Project Settings.
#include "BreathActor.h"
#include "Components/BoxComponent.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

ABreathActor::ABreathActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	ActiveTime = 0.f;
	ApplyTime = 0.f;
}

void ABreathActor::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->SetVisibility(true);
	BoxComponent->SetBoxExtent(FVector(600.f, 30.f, 62.f));

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlapped);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlapped);
}

void ABreathActor::OnBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		Player = Cast<APlayerCharacter>(OtherActor);
		IsActive = true;
	}
}

void ABreathActor::OnEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		Player = nullptr;
	}
}

void ABreathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActiveTime += DeltaTime;
	if (ActiveTime >= 0.5f)
	{
		Destroy();
	}

	if (IsActive)
	{
		ApplyTime += DeltaTime;
		if (ApplyTime > 0.2f)
		{
			if (Player)
			{
				Player->OnSkillHit(GetOwner(), this, GetCombatManager(), USkillDamageType::StaticClass());
			}
			ApplyTime = 0.f;
		}
	}
}

