// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckAttackCooldown.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"

UBTServiceCheckAttackCooldown::UBTServiceCheckAttackCooldown()
{
	bSpecialAttackCooldown = false;
	Cooltime = 0.f;
}

void UBTServiceCheckAttackCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (bSpecialAttackCooldown) return;
	
	if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
	{
		bool CanSpecialAttack = BBComp->GetValueAsBool(CanSpecialAttackKey.SelectedKeyName);
		if (!CanSpecialAttack)
		{
			bSpecialAttackCooldown = true;
			if (Cast<AMonsterCharacter>(OwnerComp.GetOwner()->GetInstigator()))
			{
				if (Cast<AMonsterCharacter>(OwnerComp.GetOwner()->GetInstigator())->GetKindOfMonster() == EKindOfMonster::Skeleton)
				{
					SetFixCooltime(BBComp);
				}
				else
				{
					SetRandCooltime(BBComp);
				}
			}
		}
	}
}

void UBTServiceCheckAttackCooldown::SetFixCooltime(UBlackboardComponent* BBComp)
{
	Cooltime = FMath::RandRange(1.f, 1.5f);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, BBComp]()
		{
				CooldownFinished(BBComp);
		}, Cooltime, false);
}

void UBTServiceCheckAttackCooldown::SetRandCooltime(UBlackboardComponent* BBComp)
{
	Cooltime = FMath::RandRange(2.f, 4.f);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, BBComp]()
		{
				CooldownFinished(BBComp);
		}, Cooltime, false);
}

void UBTServiceCheckAttackCooldown::CooldownFinished(UBlackboardComponent* BBComp)
{
	BBComp->SetValueAsBool(CanSpecialAttackKey.SelectedKeyName, true);
	bSpecialAttackCooldown = false;
}
