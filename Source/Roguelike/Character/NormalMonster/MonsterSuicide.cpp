// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterSuicide.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Roguelike.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"
#include "Roguelike/Game/RLMainGameState.h"

AMonsterSuicide::AMonsterSuicide()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_SuicideMonster"));
	if (BTObject.Succeeded())
	{
		SuicideBT = BTObject.Object;
	}
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_CharacterBlockProjectile, ECollisionResponse::ECR_Overlap);
}

void AMonsterSuicide::BeginPlay()
{
	Super::BeginPlay();

	if (BoxComp)
	{
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxBeginOverlap);
	}
}

void AMonsterSuicide::GiveBTToController()
{
	RLAIController = Cast<ARLMonsterAIController>(GetController());
	if (RLAIController && SuicideBT)
	{
		RLAIController->SetBehaviorTree(SuicideBT);
	}
}

void AMonsterSuicide::Dead()
{
	Super::Dead();
	// if (GetMovementComponent() && GetMesh() && GetCapsuleComponent())
	// {
	// 	GetMovementComponent()->SetActive(false);
	// 	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// }
	// CalcGiveBuffPer();
	// SetIsDeadBB(true);
	// SetIsDeadAnimInst(true);
	// RemoveHPWidget();
	// DeadInit();
	//
	// FTimerHandle DestroyTimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ThisClass::Deactivate, 3.f);

	if (ManagerComponent)
	{
		TArray<AActor*> IgnoreActor;
		const float ATK = ManagerComponent->GetCombatManager().ATK * 5.f;
		UGameplayStatics::ApplyRadialDamage(
			this,
			ATK,
			GetActorLocation(),
			150.f,
			USkillDamageType::StaticClass(),
			IgnoreActor,
			this,
			GetController(),
			true
		);
	}
}

void AMonsterSuicide::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsDead()) return;
	RLAIController = Cast<ARLMonsterAIController>(GetController());
	if (OtherActor)
	{
		if (RLAIController && RLAIController->GetBlackboardComponent())
		{
			AActor* Target = Cast<AActor>(RLAIController->GetBlackboardComponent()->GetValueAsObject(FName("Target")));
			if (Target == OtherActor)
			{
				Dead();
				if (Cast<ARLMainGameState>(UGameplayStatics::GetGameState(this)))
				{
					Cast<ARLMainGameState>(UGameplayStatics::GetGameState(this))->KillScored();
				}
			}
		}
	}
}


