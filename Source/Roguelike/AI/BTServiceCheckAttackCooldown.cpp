// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckAttackCooldown.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTServiceCheckAttackCooldown::UBTServiceCheckAttackCooldown()
{
	CanSpecialAttackKey.SelectedKeyName = FName("CanSpacialAttack");

	bSpecialAttackCooldown = false;
	Cooltime = 0.f;
}

void UBTServiceCheckAttackCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bSpecialAttackCooldown) return;

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp)
	{
		bool CanSpecialAttack = BBComp->GetValueAsBool(CanSpecialAttackKey.SelectedKeyName);
		if (!CanSpecialAttack)
		{
			bSpecialAttackCooldown = true;
			SetRandCooltime(BBComp);
		}
		//ƽ���� �������µ� �갡 false�� ���� ��Ÿ�� ����. 
		//�׷��� �갡 false�� bSpecialAttackCooldown�� true
		//bSpecialAttackCooldown�� true�϶� ���� ��Ÿ�� ����. �״ϱ� �ѹ��� ����Ǿ�� �Ѵ�.
		//ƽ�̶� ��� ���� �ð� ��� �����Ե�.
		//���� �� ���� bSpecialAttackCooldown�� false�� , CanAttack�� true��

	}


}

void UBTServiceCheckAttackCooldown::SetRandCooltime(UBlackboardComponent* BBComp)
{
	UE_LOG(LogTemp, Warning, TEXT("SetRandCoolTime"));
	Cooltime = FMath::RandRange(4.f, 5.f);
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("CooldownFinished"), BBComp);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Cooltime, false);
}

void UBTServiceCheckAttackCooldown::CooldownFinished(UBlackboardComponent* BBComp)
{
	UE_LOG(LogTemp, Warning, TEXT("CoolTime Finished"));
	BBComp->SetValueAsBool(CanSpecialAttackKey.SelectedKeyName, true);
	bSpecialAttackCooldown = false;
}
