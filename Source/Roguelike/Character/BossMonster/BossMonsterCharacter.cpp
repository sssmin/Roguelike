// Fill out your copyright notice in the Description page of Project Settings.
#include "BossMonsterCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NiagaraFunctionLibrary.h"

#include "Roguelike/Actor/WhirlwindActor.h"
#include "Roguelike/Actor/BreathActor.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Game/RLGameModeBase.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"
#include "Roguelike/Widget/BossHPBarWidget.h"

ABossMonsterCharacter::ABossMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bSkillFlipflop = false;
	bIsActiveBreath = false;
	SpawnedBreathNum = 0;
	BreathSpawnTime = 0.f;
	BreathDegree = 0.f;
	BreathMaxSpawnNum = 9;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_BossMonster"));
	if (BTObject.Succeeded())
	{
		BossBT = BTObject.Object;
	}
}

void ABossMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActiveBreath && (SpawnedBreathNum < BreathMaxSpawnNum))
	{
		BreathSpawnTime += DeltaTime;
		if (BreathSpawnTime > 0.1f)
		{
			SpawnBreathActor();
			BreathSpawnTime = 0.f;
		}
	}
}

void ABossMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABossMonsterCharacter::GiveBTToController()
{
	RLAIController = RLAIController == nullptr ? Cast<ARLMonsterAIController>(GetController()) : RLAIController;
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
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;

	for (int32 i = 0; i < 6; ++i)
	{
		FVector SpawnLocation = GetRandomFloor();
		FTransform SpawnTransform = FTransform(GetActorTransform().GetRotation(), SpawnLocation, FVector(1.f, 1.f, 1.f));
		if (WhirlwindActorClass)
		{
			AWhirlwindActor* WhirlwindActor = GetWorld()->SpawnActor<AWhirlwindActor>(WhirlwindActorClass, SpawnTransform, Params);
			if (WhirlwindActor && GetManagerComp())
			{
				SpawnedWhirlwindActors.Add(WhirlwindActor);
				WhirlwindActor->SetCombatManager(GetManagerComp()->GetCombatManager());
			}
		}
	}
}

void ABossMonsterCharacter::RemoveSpawnedWhirlwindActor(AWhirlwindActor* WhirlwindActor)
{
	if (SpawnedWhirlwindActors.Contains(WhirlwindActor))
	{
		SpawnedWhirlwindActors.Remove(WhirlwindActor);
	}
}

FVector ABossMonsterCharacter::GetRandomFloor()
{
	const float RandValueX = FMath::RandRange(-800.f, 800.f);
	const float RandValueY = FMath::RandRange(-800.f, 800.f);
	const FVector TargetLocation = GetActorLocation() + FVector(RandValueX, RandValueY, 0.f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TargetLocation, TargetLocation + (GetActorUpVector() * -600.f), ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		return HitResult.ImpactPoint;
	}
	return FVector::ZeroVector;
}

void ABossMonsterCharacter::SpawnBreathActor()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;
	if (SpawnedBreathNum == 0)
	{
		BreathDegree = 30.f;
	}
	FVector Dir = UKismetMathLibrary::CreateVectorFromYawPitch(GetOwner()->GetActorRotation().Yaw + BreathDegree, 0.f);

	BreathDegree -= 7.5f;

	FTransform SpawnTransform = FTransform(Dir.Rotation(), GetActorLocation() + Dir * 400.f, FVector(1.f, 1.f, 1.f));
	if (BreathActorClass)
	{
		ABreathActor* BreathActor = GetWorld()->SpawnActor<ABreathActor>(BreathActorClass, SpawnTransform, Params);
		if (BreathActor && GetManagerComp())
		{
			BreathActor->SetCombatManager(GetManagerComp()->GetCombatManager());
		}
	}
	SpawnedBreathNum++;

	if (SpawnedBreathNum == BreathMaxSpawnNum)
	{
		bIsActiveBreath = false;
		SpawnedBreathNum = 0;
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
	bIsActiveBreath = true;
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
	const FVector SpawnLoc = GetActorLocation() + GetActorForwardVector() * 100.f;
	const FVector Dir = GetActorForwardVector();

	if (GetMonsterCombatComp())
	{
		GetMonsterCombatComp()->FireToDir(SpawnLoc, Dir, USkillDamageType::StaticClass());
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
	ARLGameModeBase* RLGM = Cast<ARLGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (RLGM)
	{
		FVector CellScale = RLGM->GetBossCellScale();

		for (int32 i = 0; i < 8; ++i)
		{
			FVector SpawnRandLoc = GetRandomLoc(CellScale);
			FVector Dir = GetRandomDir(CellScale, SpawnRandLoc);
			FireToDir(SpawnRandLoc, Dir, USkillDamageType::StaticClass());
		}
	}
}

void ABossMonsterCharacter::Destroyed()
{
	Super::Destroyed();

	if (GetBossHPBarWidget())
	{
		GetBossHPBarWidget()->RemoveFromViewport();
	}
}

void ABossMonsterCharacter::SetIsDeadAnimInst()
{
	Super::SetIsDeadAnimInst();
	if (BossEgo)
	{
		BossEgo->SetIsDeadAnimInst();
	}
}

void ABossMonsterCharacter::SetIsDeadBB()
{
	Super::SetIsDeadBB();
	if (BossEgo)
	{
		BossEgo->SetIsDeadBB();
	}
}

void ABossMonsterCharacter::Dead()
{
	Super::Dead();

	for(auto Actor : SpawnedWhirlwindActors)
	{
		if (Actor)
		{
			Actor->ReserveDestroy();
		}
	}
	SpawnedWhirlwindActors.Empty();
}

void ABossMonsterCharacter::OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager,
                                  AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	Super::OnHit(EnemyCombatManager, EnemyItemManager, Attacker, DamageCauser, DamageType);

	if (BossEgo)
	{
		ARLMonsterAIController* EgoController = Cast<ARLMonsterAIController>(BossEgo->GetController());

		if (EgoController)
		{
			EgoController->SetTarget(Attacker);
		}
	}
}

FVector ABossMonsterCharacter::GetRandomLoc(const FVector& CellScale)
{
	const float X = FMath::RandRange(-(CellScale.X / 2) * 0.3, (CellScale.X / 2) * 0.3);
	const float Y = FMath::RandRange(-(CellScale.Y / 2) * 0.3, (CellScale.Y / 2) * 0.3);

	return FVector(X, Y, GetActorLocation().Z);
}

FVector ABossMonsterCharacter::GetRandomDir(const FVector& CellScale, const FVector& RandomLoc)
{
	FVector Dir = FVector::ZeroVector;
	bool bEither = FMath::RandBool();
	float CellHalfX = 0;
	float CellHalfY = 0;

	if (RandomLoc.X > CellHalfX) //-x방향
	{
		if (bEither)
		{
			Dir = FVector(-1.f, 0.f, 0.f);
		}
	}
	if (RandomLoc.X < CellHalfX) //x방향
	{
		if (bEither)
		{
			Dir = FVector(1.f, 0.f, 0.f);
		}
	}
	if (RandomLoc.Y < CellHalfY) //-y방향
	{
		if (!bEither)
		{
			Dir = FVector(0.f, -1.f, 0.f);
		}
	}
	if (RandomLoc.Y > CellHalfY) //y방향
	{
		if (!bEither)
		{
			Dir = FVector(0.f, 1.f, 0.f);
		}
	}
	return Dir;
}

void ABossMonsterCharacter::FireToDir(const FVector& SpawnLoc, const FVector& Dir, TSubclassOf<UDamageType> DamageType)
{
	if (GetMonsterCombatComp())
	{
		GetMonsterCombatComp()->FireToDir(SpawnLoc, Dir, DamageType);
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