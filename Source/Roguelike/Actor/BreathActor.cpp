// Fill out your copyright notice in the Description page of Project Settings.
#include "BreathActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Roguelike/Roguelike.h"

#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

ABreathActor::ABreathActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	ActiveTime = 0.f;
}

void ABreathActor::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->SetVisibility(true);
	BoxComponent->SetBoxExtent(FVector(350.f, 30.f, 60.f));

	OnBeginOverlapped();
}

void ABreathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ActiveTime += DeltaTime;
	
	if (ActiveTime >= 0.5f)
	{
		Destroy();
	}
}

void ABreathActor::OnBeginOverlapped()
{
	const FVector Start = GetActorLocation() + GetActorForwardVector();
	const FVector End = GetActorLocation() + GetActorForwardVector();
	const FVector HalfSize = BoxComponent->GetScaledBoxExtent();
	const FRotator Oritentation = GetActorRotation();
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectType;
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECC_CharacterBlockProjectile);
	CollisionObjectType.Add(Pawn);
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(GetOwner());
	
	FHitResult HitResult;
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		Start,
		End,
		HalfSize,
		Oritentation,
		CollisionObjectType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Blue,
		5.f
	);
	
	if(Cast<APlayerCharacter>(HitResult.GetActor()))
	{
		Cast<APlayerCharacter>(HitResult.GetActor())->OnSkillHit(GetOwner(), this, GetCombatManager(), USkillDamageType::StaticClass());
	}
}