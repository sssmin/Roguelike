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
		//틱마다 가져오는데 얘가 false면 랜덤 쿨타임 적용. 
		//그래서 얘가 false면 bSpecialAttackCooldown이 true
		//bSpecialAttackCooldown이 true일때 랜덤 쿨타임 돌림. 그니까 한번만 실행되어야 한다.
		//틱이라서 계속 돌면 시간 계속 돌리게됨.
		//쿨이 다 돌면 bSpecialAttackCooldown를 false로 , CanAttack은 true로

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
