// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Roguelike/Widget/HPBarWidget.h"
#include "Roguelike/PlayerController/RLMonsterAIController.h"
#include "Roguelike/Component/MonsterCombatComponent.h"


#include "NavigationSystem.h"


AMonsterCharacter::AMonsterCharacter()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	MonsterType = EMonsterType::NORMAL;

	HPBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComp"));
	HPBarWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	static ConstructorHelpers::FClassFinder<ARLMonsterAIController> AIController(TEXT("/Game/Blueprints/Game/BP_RLMonsterAIController"));
	if (AIController.Succeeded())
	{
		AIControllerClass = AIController.Class;
	}
	MonsterCombatComp = CreateDefaultSubobject<UMonsterCombatComponent>(TEXT("MonsterCombatComp"));
}

void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (MonsterCombatComp)
	{
		MonsterCombatComp->GetCombatManager.BindUObject(this, &ABaseCharacter::GetCombatManager);
	}
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

void AMonsterCharacter::Attack()
{
	Super::Attack();

	if (MonsterCombatComp)
	{
		MonsterCombatComp->ReadyToFire(true);
	}
}

void AMonsterCharacter::SpecialAttack(AActor* Target)
{
}

void AMonsterCharacter::FireIn3Parts()	//3갈래로 미사일 발사.
{
	if (MonsterCombatComp)
	{
		MonsterCombatComp->FireInParts(3, 45.f, 45.f);
	}
}

void AMonsterCharacter::FireIn8Parts()
{
	if (MonsterCombatComp)
	{
		MonsterCombatComp->FireInParts(8, 360.f, 45.f);
	}
}

void AMonsterCharacter::FireOneToTwo()
{
	if (MonsterCombatComp)
	{
		MonsterCombatComp->FireOneToTwo(2, 30.f, 15.f);
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
		GetWorld()->SpawnActor<AMeteorActor>(MeteorClass, TargetLocation, FRotator::ZeroRotator, Params);
	}
}

