// Fill out your copyright notice in the Description page of Project Settings.
#include "OnetoAnotherProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Interface/MonsterInterface.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

AOnetoAnotherProjectile::AOnetoAnotherProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	if (PMC)
	{
		PMC->InitialSpeed = 1200.f;
		PMC->MaxSpeed = 1700.f;
	}
}

void AOnetoAnotherProjectile::OneToTwo(const FVector& Dir)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ChangeDir"), Dir);
	float ChangeTime = 0.5f;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, ChangeTime, false);
}

void AOnetoAnotherProjectile::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor == GetOwner()) return;
		if (OtherActor->Implements<UMonsterInterface>() && GetOwner()->Implements<UMonsterInterface>()) return;

		if (Cast<ABaseCharacter>(OtherActor)) //������ ���
		{
			Cast<ABaseCharacter>(OtherActor)->OnHit(CombatManager, ItemManager, GetOwner(), this, USkillDamageType::StaticClass());
			CheckAttackerBeHealed(Cast<ABaseCharacter>(GetOwner()));
			PlayHitEffect();
		}
		else if (OtherActor != GetOwner()) //�� �ƴ� �ٸ� ����
		{
			PlayDestroyEffect();
		}
	}
}

void AOnetoAnotherProjectile::ChangeDir(const FVector& Dir)
{
	if (PMC)
	{
		FVector Dirs = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), Dir);
		PMC->SetVelocityInLocalSpace(Dir * PMC->MaxSpeed);
	}
}
