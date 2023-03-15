// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem/Public/NavigationSystem.h"

#include "Roguelike/Widget/HPBarWidget.h"
#include "Roguelike/Widget/BossHPBarWidget.h"
#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Projectile/MonsterProjectile.h"
#include "Roguelike/Actor/MeteorActor.h"
#include "Roguelike/Character/MonsterAnimInstance.h"
#include "Roguelike/Game/RLGameModeBase.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"

AMonsterCharacter::AMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	MonsterType = EMonsterType::Normal;

	HPBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComp"));
	HPBarWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FClassFinder<ARLMonsterAIController> AIController(TEXT("/Game/Blueprints/Game/BP_RLMonsterAIController"));
	if (AIController.Succeeded())
	{
		AIControllerClass = AIController.Class;
	}
	MonsterCombatComp = CreateDefaultSubobject<UMonsterCombatComponent>(TEXT("MonsterCombatComp"));
	
	DefaultSpeed = 650.f;
	PatrolSpeed = 400.f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 100.f;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_MovementMonster"));
	if (BTObject.Succeeded())
	{
		BT = BTObject.Object;
	}
}

void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (MonsterCombatComp)
	{
		MonsterCombatComp->GetCombatManager.BindUObject(this, &ABaseCharacter::GetCombatManager);
	}

	RLAIController = Cast<ARLMonsterAIController>(GetController());
}

void AMonsterCharacter::GiveBTToController()
{
	RLAIController = RLAIController == nullptr ? Cast<ARLMonsterAIController>(GetController()) : RLAIController;
	
	if (RLAIController)
	{
		RLAIController->SetBehaviorTree(BT);
	}
}

void AMonsterCharacter::OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	Super::OnHit(EnemyCombatManager, EnemyItemManager, Attacker, DamageCauser, DamageType);

	RLAIController = RLAIController == nullptr ? Cast<ARLMonsterAIController>(GetController()) : RLAIController;

	if (RLAIController)
	{
		RLAIController->SetTarget(Attacker);
	}
}

void AMonsterCharacter::SetPatrolSpeed()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	}
}

void AMonsterCharacter::SetDefaultSpeed()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	}
}

void AMonsterCharacter::RequestHeal(AActor* Requester) //Turret 시점
{
	if (IsDead()) return;
	if (Requester)
	{
		if (Cast<AMonsterCharacter>(Requester)->IsDead()) return;
		ARLMonsterAIController* AIController = Cast<ARLMonsterAIController>(GetController());
	
		if (AIController)
		{
			UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
			if (BBComp)
			{
				if (BBComp->GetValueAsBool(FName("CanHeal")))
				{
					UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(Requester);
					if (ManagerComp)
					{
						ManagerComp->Heal(50.f);
						BBComp->SetValueAsBool(FName("CanHeal"), false);
					}
				}
			}
		}
	}
}

void AMonsterCharacter::Dead()
{
	Super::Dead();

	SetIsDeadAnimInst();
	SetIsDeadBB();
	RemoveHPWidget();
	
	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ThisClass::ExecuteDestroy, 3.f);
}

void AMonsterCharacter::SetIsDeadAnimInst()
{
	if (GetMesh() && Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance())->SetIsDead((true));
	}
}

void AMonsterCharacter::SetIsDeadBB()
{
	ARLMonsterAIController* AIController = Cast<ARLMonsterAIController>(GetController());
	
	if (AIController)
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
		if (BBComp)
		{
			BBComp->SetValueAsBool("IsDead", true);
		}
	}
}

void AMonsterCharacter::FireOneToTwo(TSubclassOf<UDamageType> DamageType)
{
	if (MonsterCombatComp)
	{
		MonsterCombatComp->FireOneToTwo(2, 15.f, 30.f, DamageType);
	}
}

void AMonsterCharacter::FireIn3Parts(TSubclassOf<UDamageType> DamageType)
{
	if (MonsterCombatComp)
	{
		MonsterCombatComp->FireInParts(3, 45.f, 45.f, DamageType);
	}
}

void AMonsterCharacter::FireIn8Parts(TSubclassOf<UDamageType> DamageType)
{
	if (MonsterCombatComp)
	{
		MonsterCombatComp->FireSpreadFromCenter(8, 360.f, 45.f, DamageType);
	}
}

void AMonsterCharacter::FireSpread8PartsFromCenter(TSubclassOf<UDamageType> DamageType)
{
	if (MonsterCombatComp)
	{
		MonsterCombatComp->FireSpreadFromCenter(8, 360.f, 45.f, DamageType);
	}
}

void AMonsterCharacter::ExecuteDestroy()
{
	Destroy();
}

void AMonsterCharacter::SetHPBarWidget(const TSubclassOf<UHPBarWidget>& Widget)
{
	if (HPBarWidgetComp && Widget)
	{
		UHPBarWidget* HPBarWidget = CreateWidget<UHPBarWidget>(GetWorld(), Widget);

		if (HPBarWidget)
		{
			HPBarWidgetComp->SetWidget(HPBarWidget);
			HPBarWidget->SetOwnerPlayer(this);
		}

		HPBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		HPBarWidgetComp->SetDrawSize(FVector2D(200.f, 20.f));
		HPBarWidgetComp->SetPivot(FVector2D(0.5f, 4.5f));
	}
}

void AMonsterCharacter::SetHPBarWidget(const TSubclassOf<UBossHPBarWidget>& Widget)
{
	if (Widget)
	{
		BossHPBarWidget = CreateWidget<UBossHPBarWidget>(GetWorld(), Widget);

		if (BossHPBarWidget)
		{
			BossHPBarWidget->Init(this);
			BossHPBarWidget->AddToViewport();
		}
	}
}

void AMonsterCharacter::RemoveHPWidget()
{
	if (MonsterType == EMonsterType::Normal || MonsterType == EMonsterType::Elite)
	{
		if (HPBarWidgetComp)
		{
			HPBarWidgetComp->DestroyComponent();
		}
	}
	else
	{
		if (BossHPBarWidget)
		{
			BossHPBarWidget->RemoveFromViewport();
		}
	}
}

void AMonsterCharacter::Attack()
{
	Super::Attack();

	if (MonsterCombatComp)
	{
		const FVector SpawnLoc = GetActorLocation() + GetActorForwardVector() * 100.f;
		const FVector Dir = GetActorForwardVector();

		MonsterCombatComp->FireToDir(SpawnLoc, Dir, UDamageType::StaticClass());
	}
}

void AMonsterCharacter::SpecialAttack(AActor* Target)
{
	int32 RandValue = FMath::RandRange(0, 5);
	switch (RandValue)
	{
	case 0:
		FireIn3Parts(USkillDamageType::StaticClass());
		break;
	case 1:
		FireIn8Parts(USkillDamageType::StaticClass());
		break;
	case 2:
		FireOneToTwo(USkillDamageType::StaticClass());
		break;
	case 3:
		FireSpread8PartsFromCenter(USkillDamageType::StaticClass());
		break;
	case 4:
		Fire3Projectile(USkillDamageType::StaticClass());
		break;
	case 5:
		Meteor(GetMeteorActorClass(), Target);
		break;
	}
}

void AMonsterCharacter::ThrowBomb(AActor* Target, TSubclassOf<UDamageType> DamageType)
{
	if (MonsterCombatComp)
	{
		MonsterCombatComp->ThrowBomb(Target, DamageType);
	}
}

void AMonsterCharacter::Fire3Projectile(TSubclassOf<UDamageType> DamageType)
{
	if (MonsterCombatComp)
	{
		MonsterCombatComp->FireVariousProjectile(3, DamageType);
	}
}

void AMonsterCharacter::Teleport()
{
	FVector RandVector = UNavigationSystemV1::GetRandomPointInNavigableRadius(this, FVector::ZeroVector, 1);
	TEnumAsByte<EMoveComponentAction::Type> Val = EMoveComponentAction::Move;
	FLatentActionInfo ActionInfo;
	ActionInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, RandVector, GetActorRotation(), false, true, 0.5f, true, Val, ActionInfo);
}

void AMonsterCharacter::Meteor(const TSubclassOf<AMeteorActor>& MeteorClass, AActor* Target)
{
	if (MeteorClass && Target)
	{
		FVector TargetLocation = Target->GetActorLocation();
		TargetLocation = FVector(TargetLocation.X, TargetLocation.Y, 0.f);
		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.Instigator = this;
		AMeteorActor* Meteor = GetWorld()->SpawnActor<AMeteorActor>(MeteorClass, TargetLocation, FRotator::ZeroRotator, Params);
		if (Meteor && GetManagerComp())
		{
			Meteor->SetCombatManager(GetManagerComp()->GetCombatManager());
		}
	}
}