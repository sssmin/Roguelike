// Fill out your copyright notice in the Description page of Project Settings.
#include "BossMonsterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/ArrowComponent.h"

#include "Roguelike/Actor/WhirlwindActor.h"
#include "Roguelike/Character/NormalMonster/MonsterBossEgo.h"
#include "Roguelike/Component/BossCombatComponent.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"
#include "Roguelike/Widget/BossHPBarWidget.h"

ABossMonsterCharacter::ABossMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bSkillFlipflop = false;
	BossCombatComp = CreateDefaultSubobject<UBossCombatComponent>(TEXT("BossCombatComp"));
	if (GetMonsterCombatComp())
	{
		GetMonsterCombatComp()->DestroyComponent();
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_BossMonster"));
	if (BTObject.Succeeded())
	{
		BossBT = BTObject.Object;
	}
	BreathLocationComp = CreateDefaultSubobject<UArrowComponent>(TEXT("BreathLocationComp"));
	BreathLocationComp->SetupAttachment(RootComponent	);
}

void ABossMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (BossCombatComp)
	{
		BossCombatComp->GetCombatManager.BindUObject(this, &ABaseCharacter::GetCombatManager);
	}
}

void ABossMonsterCharacter::GiveBTToController()
{
	RLAIController = Cast<ARLMonsterAIController>(GetController());
	
	if (RLAIController)
	{
		RLAIController->SetBehaviorTree(BossBT);
	}
}

void ABossMonsterCharacter::Whirlwind()
{
	if (WhirlwindMontage)
	{
		PlayAnimMontage(WhirlwindMontage);
	}
}

void ABossMonsterCharacter::OnExecuteWhirlwind()
{
	if (BossCombatComp)
	{
		BossCombatComp->Whirlwind();
	}
}

void ABossMonsterCharacter::RemoveSpawnedWhirlwindActor(AWhirlwindActor* WhirlwindActor)
{
	if (BossCombatComp)
	{
		BossCombatComp->RemoveSpawnedWhirlwindActor(WhirlwindActor);
	}
}

void ABossMonsterCharacter::Breath()
{
	if (BreathMontage)
	{
		PlayAnimMontage(BreathMontage);
	}
}

void ABossMonsterCharacter::OnExecuteBreath()
{
	if (BossCombatComp && BreathLocationComp)
	{
		BossCombatComp->Breath(EBreathType::RADIAL, BreathLocationComp->GetComponentLocation());
	}
}

void ABossMonsterCharacter::BreathForward()
{
	if (BreathForwardMontage)
	{
		PlayAnimMontage(BreathForwardMontage);
	}
}

void ABossMonsterCharacter::OnExecuteBreathForward()
{
	if (BossCombatComp && BreathLocationComp)
	{
		BossCombatComp->Breath(EBreathType::FORWARD, BreathLocationComp->GetComponentLocation());
	}
}

void ABossMonsterCharacter::ThrowBall()
{
	if (ThrowMontage)
	{
		PlayAnimMontage(ThrowMontage);
	}
}

void ABossMonsterCharacter::OnExecuteThrowBall()
{
	if (BossCombatComp)
	{
		BossCombatComp->ThrowBall();
	}
}

void ABossMonsterCharacter::Square()
{
	if (SquareMontage)
	{
		PlayAnimMontage(SquareMontage);
	}
}

void ABossMonsterCharacter::OnExecuteSquare()
{
	if (BossCombatComp)
	{
		BossCombatComp->Square();
	}
}

void ABossMonsterCharacter::Destroyed()
{
	Super::Destroyed();

	if (GetBossHPBarWidget())
	{
		GetBossHPBarWidget()->RemoveFromParent();
	}
}

void ABossMonsterCharacter::SetIsDeadAnimInst(bool InIsDead)
{
	Super::SetIsDeadAnimInst(InIsDead);
	
	if (BossEgo)
	{
		BossEgo->SetIsDeadAnimInst(InIsDead);
	}
}

void ABossMonsterCharacter::SetIsDeadBB(bool InIsDead)
{
	Super::SetIsDeadBB(InIsDead);
	
	if (BossEgo)
	{
		BossEgo->SetIsDeadBB(InIsDead);
	}
}

void ABossMonsterCharacter::Dead()
{
	Super::Dead();

	if (BossEgo)
	{
		BossEgo->DisconnectController();
		BossEgo->Deactivate();
	}
	if (BossCombatComp)
	{
		BossCombatComp->ReserveDestroySpawnedWhirlwind();
	}
}

void ABossMonsterCharacter::OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	Super::OnHit(EnemyCombatManager, EnemyItemManager, Attacker, DamageCauser, DamageType);

	if (BossEgo)
	{
		if (ARLMonsterAIController* EgoController = Cast<ARLMonsterAIController>(BossEgo->GetController()))
		{
			EgoController->SetTarget(Attacker);
		}
	}
}

void ABossMonsterCharacter::ExecuteSkill()
{
}

void ABossMonsterCharacter::OnSkillEnd()
{
	if (RLAIController)
	{
		RLAIController->OnSkillEnd();
	}
}