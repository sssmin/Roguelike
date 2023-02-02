// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Roguelike/Projectile/BaseProjectile.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::Fire()
{
	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	if (ProjectileClass)
	{
		ABaseProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), Params);
		if (SpawnedProjectile)
		{
			SpawnedProjectile->SetVelocity(GetOwner()->GetActorForwardVector());
			//여기서 내가 가진 특성이나, 효과로 미사일에 값을 줘야한다.
		}
	}
	
}

