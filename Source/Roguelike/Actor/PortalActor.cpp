// Fill out your copyright notice in the Description page of Project Settings.
#include "PortalActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"


APortalActor::APortalActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Player_Portal, ECollisionResponse::ECR_Overlap);
	SetRootComponent(SphereComp);
}

void APortalActor::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComp)
	{
		SphereComp->Deactivate();
	}
}

void APortalActor::PortalParticleVisible(bool IsActive)
{
	if (SphereComp)
	{
		SphereComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &APortalActor::BeginOverlapped);
		SphereComp->Activate();
	}
}

void APortalActor::BeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		if (Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			switch (PortalType)
			{
			case EPortalType::SIDE:
				Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this))->RequestMove(Dir);
				break;
			case EPortalType::CENTER:
				Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this))->RequestMoveNextStage();
				break;
			case EPortalType::PREV_BOSS:
				Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this))->RequestMovePrevBossCell();
				break;
			
			}
		}
	}
}

void APortalActor::SetCenterPortal()
{
	if (CenterPortalCreateParticle && CenterPortalParticle && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CenterPortalCreateParticle, GetActorTransform());
		CenterPortalParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CenterPortalParticle, GetActorTransform());
	}
	PortalParticleVisible(true);
	PortalType = EPortalType::CENTER;
}

void APortalActor::SetSidePortal()
{
	if (PortalParticle && GetWorld())
	{
		PortalParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalParticle, GetActorTransform());
	}
	PortalParticleVisible(true);
	PortalType = EPortalType::SIDE;
}

void APortalActor::SetPrevBossPortal()
{
	if (CenterPortalCreateParticle && CenterPortalParticle && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CenterPortalCreateParticle, GetActorTransform());
		CenterPortalParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CenterPortalParticle, GetActorTransform());
	}
	PortalParticleVisible(true);
	PortalType = EPortalType::PREV_BOSS;
}

void APortalActor::Destroyed()
{
	if (PortalParticleComp)
	{
		PortalParticleComp->DestroyComponent();
	}

	if (CenterPortalParticleComp)
	{
		CenterPortalParticleComp->DestroyComponent();
	}
}