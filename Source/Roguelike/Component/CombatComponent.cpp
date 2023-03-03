// Fill out your copyright notice in the Description page of Project Settings.
#include "CombatComponent.h"
#include "GameFramework/Character.h"

#include "Roguelike/Projectile/BaseProjectile.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::ReadyToFire(bool bPressed)
{
}

void UCombatComponent::PlayFireMontage()
{
	if (Cast<ACharacter>(GetOwner()) && Cast<ACharacter>(GetOwner())->GetMesh() && Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance() && FireMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayFireMongate"));
		Cast<ACharacter>(GetOwner())->PlayAnimMontage(FireMontage, 1.f, GetRandomMontageSection());
	}
}

FName UCombatComponent::GetRandomMontageSection()
{
	FName RtnValue = NAME_None;
	if (FireMontage)
	{
		int32 RandValue = FMath::RandRange(0, FireMontage->CompositeSections.Num());
		RtnValue = FireMontage->GetSectionName(RandValue);
	}
	return RtnValue;
}