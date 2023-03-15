// Fill out your copyright notice in the Description page of Project Settings.
#include "PortalActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"

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

	InteractWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidgetComponent"));
	InteractWidgetComp->SetupAttachment(RootComponent);


	InteractSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphereComp"));
	InteractSphereComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	InteractSphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractSphereComp->SetCollisionResponseToChannel(ECC_Player_Portal, ECollisionResponse::ECR_Overlap);
	InteractSphereComp->SetupAttachment(RootComponent);

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	InteractSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractSphere->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	InteractSphere->SetupAttachment(RootComponent);

	bIsInteractActive = false;
}

void APortalActor::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComp && InteractSphereComp && InteractWidgetComp)
	{
		SphereComp->Deactivate();
		InteractSphereComp->Deactivate();
		InteractWidgetComp->SetVisibility(false);
	}
}

void APortalActor::PortalActivate()
{
	if (SphereComp && ((PortalType == EPortalType::SIDE) || (PortalType == EPortalType::CENTER)))
	{
		SphereComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &APortalActor::BeginOverlapped);
		SphereComp->Activate();
	}
	if (InteractSphereComp && (PortalType == EPortalType::PREV_BOSS))
	{
		InteractSphereComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &APortalActor::InteractBeginOverlapped);
		InteractSphereComp->OnComponentEndOverlap.AddUniqueDynamic(this, &APortalActor::InteractEndOverlapped);
		InteractSphereComp->Activate();
	}
}

void APortalActor::BeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
		if (GI)
		{
			switch (PortalType)
			{
			case EPortalType::SIDE:
				GI->RequestMove(Dir);
				break;
			case EPortalType::CENTER:
				GI->RequestMoveNextStage();
				break;
			}
		}
	}
}

void APortalActor::InteractBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InteractWidgetComp)
	{
		InteractWidgetComp->SetVisibility(true);
	}
	bIsInteractActive = true;	
}

void APortalActor::InteractEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InteractWidgetComp)
	{
		InteractWidgetComp->SetVisibility(false);
	}
	bIsInteractActive = false;
}

void APortalActor::SetCenterPortal()
{
	if (CenterPortalCreateParticle && CenterPortalParticle && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CenterPortalCreateParticle, GetActorTransform());
		CenterPortalParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CenterPortalParticle, GetActorTransform());
	}
	PortalType = EPortalType::CENTER;
	PortalActivate();
}

void APortalActor::SetSidePortal()
{
	if (PortalParticle && GetWorld())
	{
		PortalParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalParticle, GetActorTransform());
	}
	PortalType = EPortalType::SIDE;
	PortalActivate();
}

void APortalActor::SetPrevBossPortal()
{
	if (CenterPortalCreateParticle && CenterPortalParticle && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CenterPortalCreateParticle, GetActorTransform());
		CenterPortalParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CenterPortalParticle, GetActorTransform());
	}
	PortalType = EPortalType::PREV_BOSS;
	PortalActivate();
}

void APortalActor::Interact()
{
	if (bIsInteractActive)
	{
		URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
		if (GI)
		{
			GI->RequestMovePrevBossCell();
		}
	}
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