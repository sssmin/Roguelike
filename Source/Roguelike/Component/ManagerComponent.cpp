// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerComponent.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLGameStateBase.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "Roguelike/Character/PlayerCharacter.h"
#include "Roguelike/Character/BossMonsterCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ItemComponent.h"

UManagerComponent::UManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	HealthManager = FHealthManager();
	CombatManager = FCombatManager();
	CurrentState = 0;
	CurrentBuff = 0;
	CCStack = 0;
	CCStackDuration = 0.f;
	CCSeconds = 0.f;
	CCDuration = 2.5f;
	bShouldApplyCC = false;
	bIsBurn = false;
	BurnTime = 0.f;
	

}


void UManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	
	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && Cast<APlayerCharacter>(GetOwner()))
	{
		GI->GetManager(HealthManager, CombatManager, CurrentBuff);
		GI->OnMoveMap.BindUObject(this, &ThisClass::SendManager);
		ApplyBuff(CurrentBuff);
		//아이템 장착
	}
}


void UManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ManageStack(DeltaTime);
	
	if (HaveAnyState())
	{
		CCSeconds += DeltaTime;
		if (CCSeconds >= CCDuration)
		{
			CancelCC();
			CCSeconds = 0.f;
		}
		else
		{
			if (bShouldApplyCC)
			{
				ApplyCC();
			}
		}
	}

	if (bIsBurn)
	{
		BurnTime += DeltaTime;
		if (BurnTime >= 1.f)
		{
			ApplyBurnDamage();
			BurnTime = 0.f;
		}
	}
}

void UManagerComponent::SendManager()
{
	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		GI->SetManager(HealthManager, CombatManager, CurrentBuff);
	}
}

void UManagerComponent::ReceiveDamage(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager)
{
	if (CheckState(static_cast<uint8>(EState::DEAD))) return;

	float RiskReturn = 1.f;
	if (ItemComponent)
	{
		if (ItemComponent->CheckOnceItem(static_cast<uint8>(EOnceEquippedItem::RISK_RETURN)))
		{
			RiskReturn = 3.f;
		}
		if (ItemComponent->CheckOnceItem(static_cast<uint8>(EOnceEquippedItem::DODGE)))
		{
			if (IsDodge()) return;
		}
	}

	CalcCC(EnemyCombatManager);

	const float Coefficient = CalcCounter(EnemyCombatManager.Element); //계수
	const float EnemyATK = EnemyCombatManager.ATK;
	const float CriticalPer = CalcCritical(EnemyCombatManager);
	
	const float Result = FMath::Clamp((EnemyATK * Coefficient) * FMath::RandRange(0.9, 1.1) * (RiskReturn) * CriticalPer, 0.f, TNumericLimits<int32>::Max());
	UpdateCurrentHP(-Result);

	if (HealthManager.CurrentHP <= 0.f)
	{
		Dead();
	}
}

bool UManagerComponent::CheckState(uint8 State)
{
	return CurrentState & State;
}

void UManagerComponent::ApplyState(uint8 State)
{
	CurrentState |= State;
}

void UManagerComponent::RemoveState(uint8 State)
{
	CurrentState ^= State;
}

bool UManagerComponent::CheckBuff(uint8 Buff)
{
	return CurrentBuff & Buff;
}

void UManagerComponent::ApplyBuff(uint8 Buff)
{
	CurrentBuff |= Buff;

	if (Buff == static_cast<uint8>(EBuff::MOVEMENT_BUFF))
	{
		if (Cast<APlayerCharacter>(GetOwner()))
		{
			Cast<APlayerCharacter>(GetOwner())->IncreaseMovementSpeed();
		}
	}
}

void UManagerComponent::RemoveBuff(uint8 Buff)
{
	CurrentBuff ^= Buff;

	if (Buff == static_cast<uint8>(EBuff::MOVEMENT_BUFF))
	{
		if (Cast<APlayerCharacter>(GetOwner()))
		{
			Cast<APlayerCharacter>(GetOwner())->DecreaseMovementSpeed();
		}
	}
}

float UManagerComponent::CalcCounter(EElement EnemyElement)//상성. 받을 대미지 계수
{
	float Ret = 1.f;

	switch (CombatManager.Element)
	{
	case EElement::NONE:
		Ret = 1.f;
		break;
	case EElement::FIRE:
		if (EnemyElement == EElement::WATER)
		{
			Ret = 2.f;
		}
		else if (EnemyElement == EElement::EARTH)
		{
			Ret = 0.5f;
		}
		break;
	case EElement::WATER:
		if (EnemyElement == EElement::EARTH)
		{
			Ret = 2.f;
		}
		else if (EnemyElement == EElement::FIRE)
		{
			Ret = 0.5f;
		}
		break;
	case EElement::EARTH:
		if (EnemyElement == EElement::FIRE)
		{
			Ret = 2.f;
		}
		else if (EnemyElement == EElement::WATER)
		{
			Ret = 0.5f;
		}
		break;
	case EElement::DARKNESS:
		if (EnemyElement == EElement::LIGHT)
		{
			Ret = 2.f;
		}
		break;
	case EElement::LIGHT:
		if (EnemyElement == EElement::DARKNESS)
		{
			Ret = 2.f;
		}
		break;
	}
	return Ret;
}

float UManagerComponent::CalcCritical(const FCombatManager& EnemyCombatManage)
{
	if (EnemyCombatManage.Critical <= 0.f) return 1.f;

	const float RandValue = FMath::RandRange(1.f, 100.f);

	if (EnemyCombatManage.Critical >= RandValue) return 2.f;
	return 1.f;
}

void UManagerComponent::Dead()
{
	ApplyState(static_cast<uint8>(EState::DEAD));

	if (Cast<ABaseCharacter>(GetOwner()))
	{
		Cast<ABaseCharacter>(GetOwner())->Dead();
	}

	if (Cast<APlayerCharacter>(GetOwner())) //죽은게 플레이어
	{
		ARLPlayerController* PC = Cast<ARLPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController());
		if (PC)
		{
			PC->ShowGameOverWidget(); 
		}
	}
	else //죽은게 몬스터
	{
		ARLGameStateBase* RLGameState = Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this));
		if (RLGameState)
		{
			if (Cast<ABossMonsterCharacter>(GetOwner())) //죽은게 보스
			{
				RLGameState->KillBoss();
				GetOwner()->Destroy();
			}
			else //일반 몹
			{
				RLGameState->KillScored();
				GetOwner()->Destroy();
			}
		}
	}
}

void UManagerComponent::TestDead()
{
	ApplyState(static_cast<uint8>(EState::DEAD));

	if (Cast<ABaseCharacter>(GetOwner()))
	{
		Cast<ABaseCharacter>(GetOwner())->Dead();
	}

	if (Cast<APlayerCharacter>(GetOwner())) //죽은게 플레이어
	{
		ARLPlayerController* PC = Cast<ARLPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController());
		if (PC)
		{
			PC->ShowGameOverWidget();
		}
	}
	else //죽은게 몬스터
	{
		ARLGameStateBase* RLGameState = Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this));
		if (RLGameState)
		{
			if (Cast<ABossMonsterCharacter>(GetOwner())) //죽은게 보스
			{
				RLGameState->KillBoss();
				GetOwner()->Destroy();
			}
			else //일반 몹
			{
				RLGameState->KillScored();
				GetOwner()->Destroy();
			}
		}
	}
}

void UManagerComponent::TestHurt()
{
	HealthManager.CurrentHP = FMath::Clamp(HealthManager.CurrentHP - 30.f, 0.f, TNumericLimits<int32>::Max());
	CombatManager.ATK++;
}

void UManagerComponent::ApplyPlayerElement(EElement Element)
{
	InitElemBuff();
	
	CombatManager.Element = Element;

	switch (CombatManager.Element)
	{
	case EElement::FIRE:
		break;
	case EElement::WATER:
		break;
	case EElement::EARTH:
		ApplyBuff(static_cast<uint8>(EBuff::MOVEMENT_BUFF));
		break;
	case EElement::DARKNESS:
		break;
	case EElement::LIGHT:
		ApplyBuff(static_cast<uint8>(EBuff::HEAL_BUFF));
		break;
	}
	if (Cast<APlayerCharacter>(GetOwner()))
	{
		Cast<APlayerCharacter>(GetOwner())->ApplyElementParticle();
	}
	
}



void UManagerComponent::InitElemBuff()
{
	if (CombatManager.Element == EElement::EARTH)
	{
		RemoveBuff(static_cast<uint8>(EBuff::MOVEMENT_BUFF));
	}
	else if (CombatManager.Element == EElement::LIGHT)
	{
		RemoveBuff(static_cast<uint8>(EBuff::HEAL_BUFF));
	}
}

void UManagerComponent::Heal(float Rate)
{
	float CurrentHP = HealthManager.CurrentHP;
	float ToAddHP = HealthManager.MaxHP * Rate * 0.01f;
	HealthManager.CurrentHP = FMath::Clamp(CurrentHP + ToAddHP, CurrentHP, HealthManager.MaxHP);
}

void UManagerComponent::ManageStack(float DeltaTime)
{
	if (CCStack > 0)
	{
		CCStackDuration += DeltaTime;
		if (CCStackDuration >= 5.f)
		{
			CCStack = 0;
		}
	}
	else
	{
		CCStackDuration = 0.f;
	}
}

void UManagerComponent::CalcCC(const FCombatManager& EnemyCombatManager)
{
	switch (EnemyCombatManager.Element)
	{
		case EElement::FIRE:
			if (CheckState(static_cast<uint8>(EState::BURN)))
			{
				return;
			}
			CCStackDuration = 0.f;
			if (++CCStack == 4)
			{
				CCStack = 0;
				ApplyState(static_cast<uint8>(EState::BURN));
			}
			break;
		case EElement::WATER:
			if (CheckState(static_cast<uint8>(EState::FROZEN)))
			{
				return;
			}
			CCStackDuration = 0.f;
			if (++CCStack == 4)
			{
				CCStack = 0;
				ApplyState(static_cast<uint8>(EState::FROZEN));
			}
			break;
		case EElement::DARKNESS:
			if (CheckState(static_cast<uint8>(EState::FEAR)))
			{
				return;
			}
			CCStackDuration = 0.f;
			if (++CCStack == 4)
			{
				CCStack = 0;
				ApplyState(static_cast<uint8>(EState::FEAR));
			}
			break;
	}
	bShouldApplyCC = true;
}

bool UManagerComponent::HaveAnyState() //dead가 아닌 어떠한 상태
{
	return (CurrentState != 0) && !CheckState(static_cast<uint8>(EState::DEAD));
}

void UManagerComponent::ApplyCC()
{
	if (CheckState(static_cast<uint8>(EState::BURN)))
	{
		CCDuration = 5.1f;
		bIsBurn = true;
	}
	else if (CheckState(static_cast<uint8>(EState::FROZEN)))
	{
		CCDuration = 2.5f;

	}
	else if (CheckState(static_cast<uint8>(EState::FEAR)))
	{
		CCDuration = 2.5f;

	}
	bShouldApplyCC = false;
}

void UManagerComponent::CancelCC()
{
	RemoveState(CurrentBuff);
	bIsBurn = false;
}

void UManagerComponent::ApplyBurnDamage()
{
	const float MinusHP = HealthManager.MaxHP * .05f;
	HealthManager.CurrentHP = FMath::Clamp(HealthManager.CurrentHP - MinusHP, 0, HealthManager.CurrentHP);
}

bool UManagerComponent::CanAttack()
{
	return !CheckState(static_cast<uint8>(EState::FROZEN));
}

bool UManagerComponent::CanMove()
{
	return !CheckState(static_cast<uint8>(EState::FROZEN));
}


void UManagerComponent::UpdateMaxHP(float Value)
{
	if (Value != 0.f)
	{
		HealthManager.MaxHP = FMath::Clamp(HealthManager.MaxHP + Value, HealthManager.MaxHP, TNumericLimits<int32>::Max());
	}
}

void UManagerComponent::UpdateCurrentHP(float Value)
{
	if (Value != 0.f)
	{
		HealthManager.CurrentHP = FMath::Clamp(HealthManager.CurrentHP + Value, 0, HealthManager.MaxHP);
	}
}

void UManagerComponent::UpdateCurrentAtk(float Value)
{
	if (Value != 0.f)
	{
		CombatManager.ATK = FMath::Clamp(CombatManager.ATK + Value, 0, TNumericLimits<int32>::Max());
	}
}

void UManagerComponent::UpdateCurrentCritical(float Value)
{
	if (Value != 0.f)
	{
		CombatManager.Critical = FMath::Clamp(CombatManager.Critical + Value, 0.f, 10.f);
	}
}

void UManagerComponent::UpdateCurrentRange(float Value)
{
	if (Value != 0.f)
	{
		CombatManager.Range = FMath::Clamp(CombatManager.Range + Value, 900.f, 1400.f);
	}
}

bool UManagerComponent::IsDodge()
{
	float RandValue = FMath::Rand() % 100 + 1;
	if (RandValue <= 10.f)
	{
		return true;
	}
	return false;
}