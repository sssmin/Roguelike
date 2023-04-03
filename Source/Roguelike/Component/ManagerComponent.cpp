// Fill out your copyright notice in the Description page of Project Settings.
#include "ManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "ItemComponent.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLMainGameState.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Character/Player/PlayerCharacter.h"
#include "Roguelike/Character/BossMonster/BossMonsterCharacter.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

UManagerComponent::UManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Init();
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

	if (Cast<APlayerCharacter>(GetOwner()))
	{
		URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
		if (GI && GI->GetListenerManager())
		{
			GI->SetTempManageDelegate.AddUObject(this, &ThisClass::SetTempManager);
			GI->GetListenerManager()->OnLoadGameDelegate.AddUObject(this, &ThisClass::Load);
			GI->GetListenerManager()->OnNewGameDelegate.AddUObject(this, &ThisClass::Init);
			GI->GetListenerManager()->HaveCCStateDelegate.BindUObject(this, &ThisClass::HaveAnyState);
			GI->GetListenerManager()->OnUpdateCurrentAtkDelegate.BindUObject(this, &ThisClass::UpdateCurrentAtk);
			GI->GetListenerManager()->OnUpdateMaxHPDelegate.BindUObject(this, &ThisClass::UpdateMaxHP);
			GI->GetListenerManager()->OnHealByValueDelegate.BindUObject(this, &ThisClass::HealByValue);
			GI->GetListenerManager()->OnUpdateCurrentRangeDele.BindUObject(this, &ThisClass::UpdateCurrentRange);
			GI->GetListenerManager()->OnUpdateCurrentCriticalDele.BindUObject(this, &ThisClass::UpdateCurrentCritical);
		}
	}
}

void UManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	RemoveAllTimer();
}

void UManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UManagerComponent::RemoveAllTimer()
{
	RemoveTimer(BurnStackDurationTimerHandle);
	RemoveTimer(FrozenStackDurationTimerHandle);
	RemoveTimer(PoisonStackDurationTimerHandle);
	RemoveTimer(FearStackDurationTimerHandle);
	RemoveTimer(BurnStartTimerHandle);
	RemoveTimer(BurnRestTimerHandle);
	RemoveTimer(BurnApplyDamageTimerHandle);
	RemoveTimer(FrozenStartTimerHandle);
	RemoveTimer(FrozenRestTimerHandle);
	RemoveTimer(FrozenApplyDamageTimerHandle);
	RemoveTimer(PoisonStartTimerHandle);
	RemoveTimer(PoisonRestTimerHandle);
	RemoveTimer(PoisonApplyDamageTimerHandle);
	RemoveTimer(FearDurationTimerHandle);
	RemoveTimer(FearApplyDamageTimerHandle);
	RemoveTimer(MoveBuffStartTimerHandle);
	RemoveTimer(MoveBuffRestTimerHandle);
}

void UManagerComponent::RemoveTimer(FTimerHandle& Handle)
{
	if (GetWorld())
	{
		if (GetWorld()->GetTimerManager().GetTimerRemaining(Handle) >= 0.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(Handle);
		}
	}
}

void UManagerComponent::Init()
{
	HealthManager = FHealthManager();
	CombatManager = FCombatManager();
	CurrentState = 0;
	CurrentBuff = 0;
	BurnStack = 0;
	FrozenStack = 0;
	PoisonStack = 0;
	FearStack = 0;
	StateStackDuration = 5.f;
	StateSeconds = 0.f;
	StateStartDuration = 2.f;
	StateRestDuration = 3.f;
	BuffDuration = 5.f;
	StateNum = 0;
	DoTDamageRate = 0.f;
}

void UManagerComponent::Load()
{
	if (URLGameInstance* GI = URLGameInstance::GetRLGameInst(this))
	{
		GI->GetManager(HealthManager, CombatManager, CurrentBuff);
		ApplyPlayerElement(CombatManager.Element);
		ApplyBuff(CurrentBuff);
		RemoveBuff(static_cast<uint8>(EBuff::MovementBuff));
	}
}

void UManagerComponent::SetTempManager() const
{
	if (URLGameInstance* GI = URLGameInstance::GetRLGameInst(this))
	{
		GI->SetManager(HealthManager, CombatManager, CurrentBuff);
	}
}

void UManagerComponent::ReceiveDamage(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	if (IsDead()) return;

	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI && GI->GetListenerManager())
	{
		if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner()))
		{
			float RiskReturn = 1.f;
		
			
			if (GI->GetListenerManager()->CheckOnceItem(static_cast<uint8>(EOnceEquippedItem::RiskReturn)))
			{
				RiskReturn = 3.f;
			}
			if (GI->GetListenerManager()->CheckOnceItem(static_cast<uint8>(EOnceEquippedItem::Dodge)))
			{
				if (IsDodge())
				{
					OwnerCharacter->ShowNumWidget(0.f, false, false, true);
					return;
				}
			}
		

			CalcStateStack(EnemyCombatManager);

			const float Coefficient = CalcCounter(EnemyCombatManager.Element); //계수
			float EnemyATK = EnemyCombatManager.ATK;
			const float CriticalPer = CalcCritical(EnemyCombatManager);
			bool IsCritial = CriticalPer == 2.f ? true : false;
	
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
		
			OwnerCharacter->ShowNumWidget(Result, IsCritial, false, false);
			UpdateCurrentHP(-Result);
		}
	}
	
	
}

bool UManagerComponent::CheckState(uint8 State) const
{
	return CurrentState & State;
}

void UManagerComponent::SetStateTimer(float StartDuration, float RestDuration, float StateSecond, float DamageRate, FTimerHandle& StartHandle, FTimerHandle& RestHandle, FTimerHandle& DamageHandle, uint8 State, ABaseCharacter* OwnerCharacter)
{
	GetWorld()->GetTimerManager().SetTimer(StartHandle, [this, State, OwnerCharacter, &RestHandle, RestDuration]()
			{
				OwnerCharacter->FlickerStateIcon(static_cast<EState>(State));
				GetWorld()->GetTimerManager().SetTimer(RestHandle, [this, State]()
				{
					StateCompleted(State);
				}, RestDuration, false); //3초
				
			}, StartDuration, false); //2초

	//5초
	GetWorld()->GetTimerManager().SetTimer(DamageHandle, [this, DamageRate]()
		{
			ApplyDoTDamage(DamageRate);
		}, StateSecond, true);
}

void UManagerComponent::SetBuffTimer(float StartDuration, float RestDuration,  FTimerHandle& StartHandle, FTimerHandle& RestHandle,  uint8 Buff, ABaseCharacter* OwnerCharacter)
{
	GetWorld()->GetTimerManager().SetTimer(StartHandle, [this, Buff, OwnerCharacter, &RestHandle, RestDuration]()
			{
				OwnerCharacter->FlickerBuffIcon(static_cast<EBuff>(Buff));
				GetWorld()->GetTimerManager().SetTimer(RestHandle, [this, Buff]()
				{
					BuffCompleted(Buff);
				}, RestDuration, false); //3초
				
			}, StartDuration, false); //2초
}

void UManagerComponent::ApplyState(uint8 State)
{
	CurrentState |= State;
	if (IsDead()) return;
	
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		StateStartDuration = 2.f; 
		StateRestDuration = 3.f;
		StateSeconds = 1.f;
		DoTDamageRate = 0.05f;

		switch (static_cast<EState>(State))
		{
			case EState::Burn:
				SetStateTimer(StateStartDuration, StateRestDuration, StateSeconds, DoTDamageRate, BurnStartTimerHandle, BurnRestTimerHandle, BurnApplyDamageTimerHandle, State, OwnerCharacter);
				break;
			case EState::Frozen:
				SetStateTimer(StateStartDuration, StateRestDuration, StateSeconds, DoTDamageRate, FrozenStartTimerHandle, FrozenRestTimerHandle, FrozenApplyDamageTimerHandle, State, OwnerCharacter);
				break;
			case EState::Poison:
				SetStateTimer(StateStartDuration, StateRestDuration, StateSeconds, DoTDamageRate, PoisonStartTimerHandle, PoisonRestTimerHandle, PoisonApplyDamageTimerHandle, State, OwnerCharacter);
				break;
			case EState::Fear:
				{
					DoTDamageRate = 0.12f;

					if (Cast<APlayerCharacter>(OwnerCharacter))
					{
						GetWorld()->GetTimerManager().SetTimer(FearApplyDamageTimerHandle, [this]()
						{
							ApplyDoTDamage(DoTDamageRate);
						}, StateSeconds, true);
					}
				
					GetWorld()->GetTimerManager().SetTimer(FearDurationTimerHandle, [this, State]()
					{
						StateCompleted(State);
					}, StateRestDuration, false);
				}
				break;
		}

		if (OnSetStateNum.IsBound())
		{
			OnSetStateNum.Broadcast(++StateNum);	
		}
		OwnerCharacter->SetStateIcon(static_cast<EState>(State));
	}
}

void UManagerComponent::RemoveState(uint8 State)
{
	CurrentState ^= State;
	
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		OwnerCharacter->RemoveStateIcon(static_cast<EState>(State));
	}
	if (OnSetStateNum.IsBound())
	{
		OnSetStateNum.Broadcast(FMath::Clamp(--StateNum, 0, 99));	
	}
}

bool UManagerComponent::CheckBuff(uint8 Buff) const
{
	return CurrentBuff & Buff;
}

void UManagerComponent::ApplyBuff(uint8 Buff)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		CurrentBuff |= Buff;

		if (Buff == static_cast<uint8>(EBuff::MovementBuff))
		{
			Player->IncreaseMovementSpeed();
			constexpr float BuffStartDuration = 2.f; 
			constexpr float BuffRestDuration = 3.f;

			SetBuffTimer(BuffStartDuration, BuffRestDuration, MoveBuffStartTimerHandle, MoveBuffRestTimerHandle,  Buff, Cast<ABaseCharacter>(GetOwner()));
		}
		Player->SetBuffIcon(static_cast<EBuff>(Buff));
		if (OnSetStateNum.IsBound())
		{
			OnSetStateNum.Broadcast(++StateNum);	
		}
	}
}

void UManagerComponent::RemoveBuff(uint8 Buff)
{
	if (Cast<APlayerCharacter>(GetOwner()))
	{
		CurrentBuff ^= Buff;

		if (Buff == static_cast<uint8>(EBuff::MovementBuff))
		{
			Cast<APlayerCharacter>(GetOwner())->DecreaseMovementSpeed();
		}
		Cast<APlayerCharacter>(GetOwner())->RemoveBuffIcon(static_cast<EBuff>(Buff));
		if (OnSetStateNum.IsBound())
		{
			OnSetStateNum.Broadcast(FMath::Clamp(--StateNum, 0, 99));
		}
	}
}
//상성. 받을 대미지 계수
float UManagerComponent::CalcCounter(EElement EnemyElement) const
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
	if (IsDead()) return;
	RemoveAllTimer();
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
		ARLMainGameState* RLGameState = Cast<ARLMainGameState>(UGameplayStatics::GetGameState(this));
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
	CombatManager.Element = Element;
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		Player->ApplyElementParticle();
	}
}

void UManagerComponent::HealByRate(float Rate)
{
	const float CurrentHP = HealthManager.CurrentHP;
	const float MaxHP = HealthManager.MaxHP;
	float ToAddHP = HealthManager.MaxHP * Rate * 0.01f;
	
	if (MaxHP - CurrentHP < ToAddHP)
	{
		ToAddHP = MaxHP - CurrentHP;
	}
	
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		OwnerCharacter->ShowNumWidget(ToAddHP, false, true, false);
	}
	UpdateCurrentHP(ToAddHP);
}

void UManagerComponent::HealByValue(float Value)
{
	const float CurrentHP = HealthManager.CurrentHP;
	const float MaxHP = HealthManager.MaxHP;
	if (MaxHP - CurrentHP < Value)
	{
		Value = MaxHP - CurrentHP;
	}
	
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		OwnerCharacter->ShowNumWidget(Value, false, true, false);
	}
	UpdateCurrentHP(Value);
}

void UManagerComponent::InitStateStack(uint8 State)
{
	switch (static_cast<EState>(State))
	{
	case EState::Burn:
		BurnStack = 0;
		RemoveTimer(BurnStackDurationTimerHandle);
		break;
	case EState::Frozen:
		FrozenStack = 0;
		RemoveTimer(FrozenStackDurationTimerHandle);
		break;
	case EState::Poison:
		PoisonStack = 0;
		RemoveTimer(PoisonStackDurationTimerHandle);
		break;
	case EState::Fear:
		FearStack = 0;
		RemoveTimer(FearStackDurationTimerHandle);
		break;
	}
}

void UManagerComponent::CalcStateStack(const FCombatManager& EnemyCombatManager)
{
	switch (EnemyCombatManager.Element)
	{
		case EElement::Fire:
			CalcCurrentStack(static_cast<uint8>(EState::Burn), BurnStackDurationTimerHandle, BurnStack);
			break;
		case EElement::Water:
			CalcCurrentStack(static_cast<uint8>(EState::Frozen), FrozenStackDurationTimerHandle, FrozenStack);
			break;
		case EElement::Earth:
			CalcCurrentStack(static_cast<uint8>(EState::Poison), PoisonStackDurationTimerHandle, PoisonStack);
			break;
		case EElement::Darkness:
			CalcCurrentStack(static_cast<uint8>(EState::Fear), FearStackDurationTimerHandle, FearStack);
			break;
	}
}

void UManagerComponent::CalcCurrentStack(uint8 State, FTimerHandle& StackDurationTimerHandle, int32& CurrentStack)
{
	if (CheckState(State))
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(StackDurationTimerHandle, [this, State]()
		{
			InitStateStack(State);
		}, StateStackDuration, false);
	if (++CurrentStack == 4)
	{
		InitStateStack(State);
		ApplyState(State);
	}
}

bool UManagerComponent::HaveAnyState() //dead가 아닌 어떠한 상태
{
	return (CurrentState != 0) && !IsDead();
}

void UManagerComponent::StateCompleted(uint8 CompleteState)
{
	switch (static_cast<EState>(CompleteState))
	{
	case EState::Burn:
		RemoveTimer(BurnApplyDamageTimerHandle);
		break;
	case EState::Frozen:
		RemoveTimer(FrozenApplyDamageTimerHandle);
		break;
	case EState::Poison:
		RemoveTimer(PoisonApplyDamageTimerHandle);
		break;
	case EState::Fear:
		RemoveTimer(FearApplyDamageTimerHandle);
		break;
	}
	RemoveState(CompleteState);
}

void UManagerComponent::BuffCompleted(uint8 CompleteBuff)
{
	RemoveBuff(CompleteBuff);
}

void UManagerComponent::InitState() 
{
	RemoveState(CurrentState);
}

void UManagerComponent::ApplyDoTDamage(float Rate) //고정 대미지.
{
	if (IsDead()) return;
	const float MinusHP = HealthManager.MaxHP * Rate; 
	UpdateCurrentHP(-MinusHP);
	
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner()))
	{
		OwnerCharacter->ShowNumWidget(MinusHP, false, false, false); 
	}
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
	if (IsDead()) return;
	if (Value != 0.f)
	{
		HealthManager.MaxHP = FMath::Clamp(HealthManager.MaxHP + Value, HealthManager.MaxHP, TNumericLimits<int32>::Max());
		if (OnUpdateCurrentHP.IsBound())
		{
			OnUpdateCurrentHP.Broadcast(HealthManager.CurrentHP, HealthManager.MaxHP);
		}
	}
	if (Value > 0.f)
	{
		if (ARLPlayerController* PC = Cast<ARLPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController()))
		{
			PC->ShowStatNoticeWidget(EINFStackItem::IncreaseMaxHp, TEXT("최대 체력"), HealthManager.MaxHP, Value); 
		}
	}
}

void UManagerComponent::UpdateCurrentHP(float Value)
{
	if (IsDead()) return;
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

void UManagerComponent::UpdateCurrentAtk(float Value)
{
	if (Value != 0.f)
	{
		CombatManager.ATK = FMath::Clamp(CombatManager.ATK + Value, 0, TNumericLimits<int32>::Max());
	}
	if (Value > 0.f)
	{
		if (ARLPlayerController* PC = Cast<ARLPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController()))
		{
			PC->ShowStatNoticeWidget(EINFStackItem::IncreaseAtk, TEXT("공격력"), CombatManager.ATK, Value); 
		}
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

void UManagerComponent::HPSync()
{
	if (OnUpdateCurrentHP.IsBound())
	{
		OnUpdateCurrentHP.Broadcast(HealthManager.CurrentHP, HealthManager.MaxHP);
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

void UManagerComponent::ApplyMovementBuff()
{
	ApplyBuff(static_cast<uint8>(EBuff::MovementBuff));
}
