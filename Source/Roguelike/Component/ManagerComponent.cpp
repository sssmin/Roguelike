// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerComponent.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLGameStateBase.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "Roguelike/Character/PlayerCharacter.h"
#include "Roguelike/Character/BossMonsterCharacter.h"
#include "Kismet/GameplayStatics.h"

UManagerComponent::UManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	HealthManager = FHealthManage();
	CombatManager = FCombatManage();
	ItemManager = FItemManage();
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
		GI->GetManager(HealthManager, CombatManager, ItemManager, CurrentBuff);
		GI->OnMoveMap.BindUObject(this, &ThisClass::SendManager);
		ApplyBuff(CurrentBuff);
		//아이템 장착
		ApplyItem();
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
		GI->SetManager(HealthManager, CombatManager, ItemManager, CurrentBuff);
	}
}

void UManagerComponent::ReceiveDamage(const FCombatManage& EnemyCombatManage, const FItemManage& EnemyItemManage)
{
	if (CheckState(static_cast<uint8>(EState::DEAD))) return;

	CalcCC(EnemyCombatManage);

	const float Coefficient = CalcCounter(EnemyCombatManage.Element); //계수
	const float EnemyATK = EnemyCombatManage.ATK;
	const float RiskReturn = CheckItem(static_cast<uint8>(EOnceEquippedItem::RISK_RETURN)) ? 3.f : 1.f; //받는 대미지 증가

	const float Result = FMath::Clamp((EnemyATK * Coefficient) * FMath::RandRange(0.9, 1.1) * (RiskReturn), 0.f, TNumericLimits<int32>::Max());
	HealthManager.CurrentHP = FMath::Clamp(HealthManager.CurrentHP - Result, 0.f, TNumericLimits<int32>::Max());

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

bool UManagerComponent::CheckItem(uint8 Item)
{
	return ItemManager.OnceEquipedItem & Item;
}

void UManagerComponent::ApplyItem(uint8 Item)
{
	ItemManager.OnceEquipedItem |= Item;
}

void UManagerComponent::RemoveItem(uint8 Item)
{
	ItemManager.OnceEquipedItem ^= Item;
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
	//CurrentBuff = 1;
}

void UManagerComponent::ApplyPlayerElement(int32 ConvertElement)
{
	CovertElementFromInt(ConvertElement);
	
}

void UManagerComponent::CovertElementFromInt(int32 ConvertElement)
{
	InitElemBuff();

	switch (ConvertElement)
	{
		case 0:
			CombatManager.Element = EElement::FIRE;
			break;
		case 1:
			CombatManager.Element = EElement::WATER;
			break;
		case 2:
			CombatManager.Element = EElement::EARTH;
			ApplyBuff(static_cast<uint8>(EBuff::MOVEMENT_BUFF));
			break;
		case 3:
			CombatManager.Element = EElement::DARKNESS;
			break;
		case 4:
			CombatManager.Element = EElement::LIGHT;
			ApplyBuff(static_cast<uint8>(EBuff::HEAL_BUFF));
			break;
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

void UManagerComponent::CalcCC(const FCombatManage& EnemyCombatManage)
{
	switch (EnemyCombatManage.Element)
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

void UManagerComponent::ApplyItem()
{

}

