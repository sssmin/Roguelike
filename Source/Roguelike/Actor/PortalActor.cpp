// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Character/PlayerCharacter.h"
#include "Particles/ParticleSystemComponent.h"


APortalActor::APortalActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Player_Portal, ECollisionResponse::ECR_Overlap);
	IsCenterPortal = false;
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
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APortalActor::BeginOverlapped);
		SphereComp->Activate();
	}
}

void APortalActor::BeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		if (Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			if (!IsCenterPortal)
			{
				Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this))->RequestMove(Dir);
			}
			else
			{
				Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this))->RequestMoveNextStage();
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
	IsCenterPortal = true;
}

void APortalActor::SetSidePortal()
{
	if (PortalParticle && GetWorld())
	{
		PortalParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalParticle, GetActorTransform());
	}
	PortalParticleVisible(true);
	IsCenterPortal = false;
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