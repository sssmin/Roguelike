// Fill out your copyright notice in the Description page of Project Settings.
#include "ManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "ItemComponent.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLGameStateBase.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Character/BossMonster/BossMonsterCharacter.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

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

UManagerComponent* UManagerComponent::GetManagerComp(AActor* Character)
{
	if (Character)
	{
		return Character->FindComponentByClass<UManagerComponent>();
	}
	return nullptr;
}

void UManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);

	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI && GI->GetListenerManager())
	{
		GI->SetTempManageDelegate.AddUObject(this, &ThisClass::SetTempManager);
		GI->GetListenerManager()->OnLoadGameDelegate.AddUObject(this, &ThisClass::Init);
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

void UManagerComponent::Init()
{
	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI)
	{
		GI->GetManager(HealthManager, CombatManager, CurrentBuff);
		ApplyPlayerElement(CombatManager.Element);
		ApplyBuff(CurrentBuff);
	}
}

void UManagerComponent::SetTempManager() const
{
	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI)
	{
		GI->SetManager(HealthManager, CombatManager, CurrentBuff);
	}
}

void UManagerComponent::ReceiveDamage(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	if (CheckState(static_cast<uint8>(EState::Dead))) return;
	
	float RiskReturn = 1.f;
	if (ItemComponent)
	{
		if (ItemComponent->CheckOnceItem(static_cast<uint8>(EOnceEquippedItem::RiskReturn)))
		{
			RiskReturn = 3.f;
		}
		if (ItemComponent->CheckOnceItem(static_cast<uint8>(EOnceEquippedItem::Dodge)))
		{
			if (IsDodge()) return;
		}
	}

	CalcCC(EnemyCombatManager);

	const float Coefficient = CalcCounter(EnemyCombatManager.Element); //계수
	float EnemyATK = EnemyCombatManager.ATK;
	const float CriticalPer = CalcCritical(EnemyCombatManager);
	
	if (Cast<USkillDamageType>(DamageType.GetDefaultObject()))
	{
		EnemyATK *= 5.f;
	}
	else if (Cast<USpecialATKDamageType>(DamageType.GetDefaultObject()))
	{
		EnemyATK *= 2.f;
	}
	
	float Result = FMath::Clamp((EnemyATK * Coefficient) * FMath::RandRange(0.9, 1.1) * (RiskReturn) * CriticalPer, 0.f, TNumericLimits<int32>::Max());
	if (Cast<UMaxHPRatioDamageType>(DamageType.GetDefaultObject()))
	{
		Result += (HealthManager.MaxHP * 0.1f);
	}
	else if (Cast<UCurrentHPRatioDamageType>(DamageType.GetDefaultObject()))
	{
		Result += (HealthManager.CurrentHP * 0.1f);
	}
	else if (Cast<ULostHPRatioDamageType>(DamageType.GetDefaultObject()))
	{
		Result += (HealthManager.MaxHP - HealthManager.CurrentHP * 0.1f);
	}
	
	UpdateCurrentHP(-Result);
}

bool UManagerComponent::CheckState(uint8 State) const
{
	return CurrentState & State;
}

void UManagerComponent::ApplyState(uint8 State)
{
	CurrentState |= State;
	bShouldApplyCC = true;
}

void UManagerComponent::RemoveState(uint8 State)
{
	CurrentState ^= State;
}

bool UManagerComponent::CheckBuff(uint8 Buff) const
{
	return CurrentBuff & Buff;
}

void UManagerComponent::ApplyBuff(uint8 Buff)
{
	CurrentBuff |= Buff;

	if (Buff == static_cast<uint8>(EBuff::MovementBuff))
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

	if (Buff == static_cast<uint8>(EBuff::MovementBuff))
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
	case EElement::None:
		Ret = 1.f;
		break;
	case EElement::Fire:
		if (EnemyElement == EElement::Water)
		{
			Ret = 2.f;
		}
		else if (EnemyElement == EElement::Earth)
		{
			Ret = 0.5f;
		}
		break;
	case EElement::Water:
		if (EnemyElement == EElement::Earth)
		{
			Ret = 2.f;
		}
		else if (EnemyElement == EElement::Fire)
		{
			Ret = 0.5f;
		}
		break;
	case EElement::Earth:
		if (EnemyElement == EElement::Fire)
		{
			Ret = 2.f;
		}
		else if (EnemyElement == EElement::Water)
		{
			Ret = 0.5f;
		}
		break;
	case EElement::Darkness:
		if (EnemyElement == EElement::Light)
		{
			Ret = 2.f;
		}
		break;
	case EElement::Light:
		if (EnemyElement == EElement::Darkness)
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
	ApplyState(static_cast<uint8>(EState::Dead));

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
			}
			else //일반 몹
			{
				RLGameState->KillScored();
			}
		}
	}
}

bool UManagerComponent::IsDead()
{
	return CheckState(static_cast<uint8>(EState::Dead));
}

bool UManagerComponent::IsHPLow()
{
	return (HealthManager.CurrentHP / HealthManager.MaxHP * 100.f) < 45.f ? true : false;
}

void UManagerComponent::ApplyPlayerElement(EElement Element)
{
	InitElemBuff();
	
	CombatManager.Element = Element;

	switch (CombatManager.Element)
	{
	case EElement::Fire:
		break;
	case EElement::Water:
		break;
	case EElement::Earth:
		ApplyBuff(static_cast<uint8>(EBuff::MovementBuff));
		break;
	case EElement::Darkness:
		break;
	case EElement::Light:
		ApplyBuff(static_cast<uint8>(EBuff::HealBuff));
		break;
	}
	if (Cast<APlayerCharacter>(GetOwner()))
	{
		Cast<APlayerCharacter>(GetOwner())->ApplyElementParticle();
	}
}

void UManagerComponent::InitElemBuff()
{
	if (CombatManager.Element == EElement::Earth)
	{
		RemoveBuff(static_cast<uint8>(EBuff::MovementBuff));
	}
	else if (CombatManager.Element == EElement::Light)
	{
		RemoveBuff(static_cast<uint8>(EBuff::HealBuff));
	}
}

void UManagerComponent::Heal(float Rate)
{
	const float CurrentHP = HealthManager.CurrentHP;
	const float ToAddHP = HealthManager.MaxHP * Rate * 0.01f;
	UpdateCurrentHP(CurrentHP + ToAddHP);
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

void UManagerComponent::InitCCStack()
{
	CCStack = 0;
}

void UManagerComponent::CalcCC(const FCombatManager& EnemyCombatManager)
{
	switch (EnemyCombatManager.Element)
	{
		case EElement::Fire:
			if (CheckState(static_cast<uint8>(EState::Burn)))
			{
				return;
			}
			CCStackDuration = 0.f;
			if (++CCStack == 4)
			{
				InitCCStack();
				ApplyState(static_cast<uint8>(EState::Burn));
			}
			break;
		case EElement::Water:
			if (CheckState(static_cast<uint8>(EState::Frozen)))
			{
				return;
			}
			CCStackDuration = 0.f;
			if (++CCStack == 4)
			{
				InitCCStack();
				ApplyState(static_cast<uint8>(EState::Frozen));
			}
			break;
		case EElement::Darkness:
			if (CheckState(static_cast<uint8>(EState::Fear)))
			{
				return;
			}
			CCStackDuration = 0.f;
			if (++CCStack == 4)
			{
				InitCCStack();
				ApplyState(static_cast<uint8>(EState::Fear));
			}
			break;
	}
}

bool UManagerComponent::HaveAnyState() //dead가 아닌 어떠한 상태
{
	return (CurrentState != 0) && !CheckState(static_cast<uint8>(EState::Dead));
}

void UManagerComponent::ApplyCC()
{
	if (CheckState(static_cast<uint8>(EState::Burn)))
	{
		CCDuration = 5.1f;
		bIsBurn = true;
	}
	else if (CheckState(static_cast<uint8>(EState::Frozen)))
	{
		CCDuration = 2.5f;

	}
	else if (CheckState(static_cast<uint8>(EState::Fear)))
	{
		CCDuration = 2.5f;
	}
	bShouldApplyCC = false;
}

void UManagerComponent::CancelCC()
{
	RemoveState(CurrentState);
	bIsBurn = false;
}

void UManagerComponent::ApplyBurnDamage()
{
	const float MinusHP = HealthManager.MaxHP * .05f;
	UpdateCurrentHP(-MinusHP);
}

bool UManagerComponent::CanAttack() const
{
	return !CheckState(static_cast<uint8>(EState::Frozen));
}

bool UManagerComponent::CanMove() const
{
	return !CheckState(static_cast<uint8>(EState::Frozen));
}

void UManagerComponent::UpdateMaxHP(float Value)
{
	if (Value != 0.f)
	{
		HealthManager.MaxHP = FMath::Clamp(HealthManager.MaxHP + Value, HealthManager.MaxHP, TNumericLimits<int32>::Max());
		if (OnUpdateCurrentHP.IsBound())
		{
			OnUpdateCurrentHP.Broadcast(HealthManager.CurrentHP, HealthManager.MaxHP);
		}
	}
}

void UManagerComponent::UpdateCurrentHP(float Value)
{
	if (Value != 0.f)
	{
		HealthManager.CurrentHP = FMath::Clamp(HealthManager.CurrentHP + Value, 0, HealthManager.MaxHP);
		if (OnUpdateCurrentHP.IsBound())
		{
			OnUpdateCurrentHP.Broadcast(HealthManager.CurrentHP, HealthManager.MaxHP);
		}
		if (HealthManager.CurrentHP <= 0.f)
		{
			Dead();
		}
	}
}

void UManagerComponent::HPSync()
{
	if (OnUpdateCurrentHP.IsBound())
	{
		OnUpdateCurrentHP.Broadcast(HealthManager.CurrentHP, HealthManager.MaxHP);
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
	const float RandValue = FMath::Rand() % 100 + 1;
	if (RandValue <= 10.f)
	{
		return true;
	}
	return false;
}