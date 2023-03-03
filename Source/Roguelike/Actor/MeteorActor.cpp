// Fill out your copyright notice in the Description page of Project Settings.
#include "MeteorActor.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "NiagaraSystem.h"

AMeteorActor::AMeteorActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	MeteorTime = 0.5f;
}

void AMeteorActor::BeginPlay()
{
	Super::BeginPlay();

	check(NiagaraComponent);
	NiagaraComponent->Deactivate();
	NiagaraComponent->SetNiagaraVariableObject(TEXT("User.ExplodeCallback"), this);
	
	FTimerHandle MeteorTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(MeteorTimerHandle, this, &ThisClass::Active, MeteorTime, false);
	OwnerActor = GetOwner();
}

void AMeteorActor::Active()
{
	NiagaraComponent->Activate();
}




