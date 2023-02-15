// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Roguelike/Widget/HPBarWidget.h"
#include "Roguelike/PlayerController/RLMonsterAIController.h"

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

}

void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AIControllerClass)
	{
		AIControllerClass.GetDefaultObject()->Possess(this);
	}

}

void AMonsterCharacter::SetHPBarWidget(TSubclassOf<UHPBarWidget> Widget)
{
	if (HPBarWidgetComp && Widget)
	{
		HPBarWidgetComp->SetWidgetClass(Widget);
		if (Cast<UHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject()))
		{
			Cast<UHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject())->SetOwnerPlayer(this);
		}
		
		HPBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		HPBarWidgetComp->SetDrawSize(FVector2D(200.f, 20.f));
		HPBarWidgetComp->SetPivot(FVector2D(0.5f, 4.5f));
	}
}
