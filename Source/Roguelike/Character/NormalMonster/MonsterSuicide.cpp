// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterSuicide.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Roguelike.h"
#include "Roguelike/Type/DamageType/AllDamageTypes.h"
#include "Roguelike/Game/RLGameStateBase.h"

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
	RLAIController = RLAIController == nullptr ? Cast<ARLMonsterAIController>(GetController()) : RLAIController;
	if (RLAIController)
	{
		RLAIController->SetBehaviorTree(SuicideBT);
	}
}

void AMonsterSuicide::Dead()
{
	//Super::Dead();

	//소리 파티클 재생
    	
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

	Destroy();
}

void AMonsterSuicide::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsDead()) return;
	RLAIController = RLAIController == nullptr ? Cast<ARLMonsterAIController>(GetController()) : RLAIController;
	if (OtherActor)
	{
		if (RLAIController && RLAIController->GetBlackboardComponent())
		{
			AActor* Target = Cast<AActor>(RLAIController->GetBlackboardComponent()->GetValueAsObject(FName("Target")));
			if (Target == OtherActor)
			{
				Dead();
				if (Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this)))
				{
					Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this))->KillScored();
				}
			}
		}
	}
}


