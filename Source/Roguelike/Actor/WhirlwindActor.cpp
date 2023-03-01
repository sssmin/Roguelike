// Fill out your copyright notice in the Description page of Project Settings.
#include "WhirlwindActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Roguelike/Type/DamageType/AllDamageTypes.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"

AWhirlwindActor::AWhirlwindActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	IsActive = false;
	ActiveTime = 2.f;
	ApplyTime = 0.f;
	DestoryTime = 10.f;
}

void AWhirlwindActor::BeginPlay()
{
	Super::BeginPlay();
	
	

	FTimerHandle ActiveTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ActiveTimerHandle, this, &ThisClass::ActiveThisSkill, ActiveTime, false);
	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ThisClass::DestroyThisSkill, DestoryTime, false);
	if (BoxComponent)
	{
		BoxComponent->Deactivate();
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::BoxBeginOverlapped);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::BoxEndOverlapped);
	}
}

void AWhirlwindActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive)
	{
		ApplyTime += DeltaTime;
		if (ApplyTime > 0.5f)
		{
			if (Player)
			{
				Player->OnSkillHit(GetOwner(), this, GetCombatManager(), UMaxHPRatioDamageType::StaticClass());
			}
			ApplyTime = 0.f;
		}
	}
}

void AWhirlwindActor::ActiveThisSkill()
{
	IsActive = true;
	if (WhirlwindParticle && BoxComponent)
	{
		ParticleSystemComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WhirlwindParticle, GetActorTransform(), false);
		BoxComponent->Activate();
	}
	
}

void AWhirlwindActor::DestroyThisSkill()
{
	if (ParticleSystemComp)
	{
		ParticleSystemComp->DestroyComponent();
	}
	Destroy();
}

void AWhirlwindActor::BoxBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		Player = Cast<APlayerCharacter>(OtherActor);
	}
}

void AWhirlwindActor::BoxEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		Player = nullptr;
	}
}

