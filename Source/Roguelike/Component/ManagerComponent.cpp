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

	//String LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	if (Cast<APlayerCharacter>(GetOwner()))
	{
		URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
		if (GI && GI->GetListenerManager())
		{
			GI->SetTempManageDelegate.AddUObject(this, &ThisClass::SetTempManager);
			GI->GetListenerManager()->OnLoadGameDelegate.AddUObject(this, &ThisClass::Load);
			GI->GetListenerManager()->OnNewGameDelegate.AddUObject(this, &ThisClass::Init);
			GI->GetListenerManager()->HaveCCStateDelegate.BindUObject(this, &ThisClass::HaveAnyState);
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

void UManagerComponent::RemoveTimer(FTimerHandle Handle)
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
	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI)
	{
		GI->GetManager(HealthManager, CombatManager, CurrentBuff);
		ApplyPlayerElement(CombatManager.Element);
		ApplyBuff(CurrentBuff);
		RemoveBuff(static_cast<uint8>(EBuff::MovementBuff));
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
	if (IsDead()) return;
	
	float RiskReturn = 1.f;

	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());

	if (OwnerCharacter)
	{
		if (ItemComponent)
		{
			if (ItemComponent->CheckOnceItem(static_cast<uint8>(EOnceEquippedItem::RiskReturn)))
			{
				RiskReturn = 3.f;
			}
			if (ItemComponent->CheckOnceItem(static_cast<uint8>(EOnceEquippedItem::Dodge)))
			{
				if (IsDodge())
				{
					OwnerCharacter->ShowNumWidget(0.f, false, false, true);
					return;
				}
				
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
	
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		if (State  == static_cast<uint8>(EState::Burn))
		{
			StateStartDuration = 2.f; 
			StateRestDuration = 3.f;
			StateSeconds = 1.f;
			DoTDamageRate = 0.05f;
			
			SetStateTimer(StateStartDuration, StateRestDuration, StateSeconds, DoTDamageRate, BurnStartTimerHandle, BurnRestTimerHandle, BurnApplyDamageTimerHandle, State, OwnerCharacter);
		}
		else if (State  == static_cast<uint8>(EState::Frozen))
		{
			StateStartDuration = 2.f; 
			StateRestDuration = 3.f;
			StateSeconds = 1.f;
			DoTDamageRate = 0.05f;
			
			SetStateTimer(StateStartDuration, StateRestDuration, StateSeconds, DoTDamageRate, FrozenStartTimerHandle, FrozenRestTimerHandle, FrozenApplyDamageTimerHandle, State, OwnerCharacter);
		}
		else if (State  == static_cast<uint8>(EState::Poison))
		{
			StateStartDuration = 2.f; 
			StateRestDuration = 3.f;
			StateSeconds = 1.f;
			DoTDamageRate = 0.05f;
		
			SetStateTimer(StateStartDuration, StateRestDuration, StateSeconds, DoTDamageRate, PoisonStartTimerHandle, PoisonRestTimerHandle, PoisonApplyDamageTimerHandle, State, OwnerCharacter);
		}
		else if (State  == static_cast<uint8>(EState::Fear))
		{
			StateRestDuration = 3.f;
			DoTDamageRate = 0.12f;
			StateSeconds = 1.f;

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
	}

	if (OnSetStateNum.IsBound())
	{
		OnSetStateNum.Broadcast(++StateNum);	
	}
	OwnerCharacter->SetStateIcon(static_cast<EState>(State));
}

void UManagerComponent::RemoveState(uint8 State)
{
	CurrentState ^= State;

	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->RemoveStateIcon(static_cast<EState>(State));
	}
	if (OnSetStateNum.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("?"));
		OnSetStateNum.Broadcast(FMath::Clamp(--StateNum, 0, 99));	
	}
}

bool UManagerComponent::CheckBuff(uint8 Buff) const
{
	return CurrentBuff & Buff;
}

void UManagerComponent::ApplyBuff(uint8 Buff)
{
	if (Cast<APlayerCharacter>(GetOwner()))
	{
		CurrentBuff |= Buff;

		if (Buff == static_cast<uint8>(EBuff::MovementBuff))
		{
			Cast<APlayerCharacter>(GetOwner())->IncreaseMovementSpeed();
			const float BuffStartDuration = 2.f; 
			const float BuffRestDuration = 3.f;

			SetBuffTimer(BuffStartDuration, BuffRestDuration, MoveBuffStartTimerHandle, MoveBuffRestTimerHandle,  Buff, Cast<ABaseCharacter>(GetOwner()));
			
		}
		Cast<APlayerCharacter>(GetOwner())->SetBuffIcon(static_cast<EBuff>(Buff));
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
			UE_LOG(LogTemp, Warning, TEXT("?"));
			OnSetStateNum.Broadcast(FMath::Clamp(--StateNum, 0, 99));
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
	InitElemBuff();
	
	CombatManager.Element = Element;
	
	if (Cast<APlayerCharacter>(GetOwner()))
	{
		Cast<APlayerCharacter>(GetOwner())->ApplyElementParticle();
	}
}

void UManagerComponent::InitElemBuff()
{
	if (CombatManager.Element == EElement::Earth)
	{
		//RemoveBuff(static_cast<uint8>(EBuff::MovementBuff));
	}
	else if (CombatManager.Element == EElement::Light)
	{
		//RemoveBuff(static_cast<uint8>(EBuff::HealBuff));
	}
}

void UManagerComponent::HealByRate(float Rate)
{
	const float CurrentHP = HealthManager.CurrentHP;
	float ToAddHP = HealthManager.MaxHP * Rate * 0.01f;
	const float MaxHP = HealthManager.MaxHP;
	if (MaxHP - CurrentHP < ToAddHP)
	{
		ToAddHP = MaxHP - CurrentHP;
	}
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (OwnerCharacter)
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
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (OwnerCharacter)
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
			if (CheckState(static_cast<uint8>(EState::Burn)))
			{
				return;
			}
			GetWorld()->GetTimerManager().SetTimer(BurnStackDurationTimerHandle, [this]()
				{
					InitStateStack(static_cast<uint8>(EState::Burn));
				}, StateStackDuration, false);
			if (++BurnStack == 4)
			{
				InitStateStack(static_cast<uint8>(EState::Burn)); //위 타이머 제거.
				ApplyState(static_cast<uint8>(EState::Burn));
			}
			break;
		case EElement::Water:
			if (CheckState(static_cast<uint8>(EState::Frozen)))
			{
				return;
			}
			GetWorld()->GetTimerManager().SetTimer(FrozenStackDurationTimerHandle, [this]()
				{
					InitStateStack(static_cast<uint8>(EState::Frozen));
				}, StateStackDuration, false);
			if (++FrozenStack == 4)
			{
				InitStateStack(static_cast<uint8>(EState::Frozen)); //위 타이머 제거.
				ApplyState(static_cast<uint8>(EState::Frozen));
			}
			break;
		case EElement::Earth:
			if (CheckState(static_cast<uint8>(EState::Poison)))
			{
				return;
			}
			GetWorld()->GetTimerManager().SetTimer(PoisonStackDurationTimerHandle, [this]()
				{
					InitStateStack(static_cast<uint8>(EState::Poison));
				}, StateStackDuration, false);
			if (++PoisonStack == 4)
			{
				InitStateStack(static_cast<uint8>(EState::Poison)); //위 타이머 제거.
				ApplyState(static_cast<uint8>(EState::Poison));
			}
		break;
		case EElement::Darkness:
			if (CheckState(static_cast<uint8>(EState::Fear)))
			{
				return;
			}
			GetWorld()->GetTimerManager().SetTimer(FearStackDurationTimerHandle, [this]()
				{
					InitStateStack(static_cast<uint8>(EState::Fear));
				}, StateStackDuration, false);
			if (++FearStack == 4)
			{
				InitStateStack(static_cast<uint8>(EState::Fear)); //위 타이머 제거.
				ApplyState(static_cast<uint8>(EState::Fear));
			}
		break;
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
	switch (static_cast<EBuff>(CompleteBuff))
	{
	case EBuff::MovementBuff:
		//RemoveTimer(MoveBuffDurationTimerHandle);
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("BuffC"));
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
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (OwnerCharacter)
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
		ARLPlayerController* PC = Cast<ARLPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController());
		if (PC)
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
	if (Value > 0.f)
	{
		ARLPlayerController* PC = Cast<ARLPlayerController>(Cast<APlayerCharacter>(GetOwner())->GetController());
		if (PC)
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
