// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLGameStateBase.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "Roguelike/Type/ItemManage.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Component/ItemComponent.h"
#include "Roguelike/Component/PlayerCombatComponent.h"

#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player_Portal"));

	bPressedAttackButton = false;

	ItemComp = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComponent"));
	PlayerCombatComp = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComp"));
	
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		PC = GetWorld()->GetFirstPlayerController();
	}
	if (PC)
	{
		FInputModeGameAndUI InputModeData;
		InputModeData.SetHideCursorDuringCapture(false);
		//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
		PC->SetInputMode(InputModeData);
	}

	if (ManagerComponent && ItemComp)
	{
		ManagerComponent->SetItemComp(ItemComp);
		ItemComp->SetManagerComp(ManagerComponent);
	}

	if (PlayerCombatComp)
	{
		PlayerCombatComp->GetItemManager.BindUObject(this, &ThisClass::GetItemManager);
		PlayerCombatComp->GetCombatManager.BindUObject(this, &ABaseCharacter::GetCombatManager);
	}
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Test1", IE_Pressed, this, &ThisClass::Test1);
	PlayerInputComponent->BindAction("Test2", IE_Pressed, this, &ThisClass::Test2);
	PlayerInputComponent->BindAction("Test3", IE_Pressed, this, &ThisClass::Test3);

	PlayerInputComponent->BindAction("Recall", IE_Pressed, this, &ThisClass::Recall);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ThisClass::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ThisClass::AttackReleased);

	PlayerInputComponent->BindAction("FreeCam", IE_Pressed, this, &ThisClass::PressedFreeCam);
	PlayerInputComponent->BindAction("FreeCam", IE_Released, this, &ThisClass::ReleasedFreeCam);

}

void APlayerCharacter::PressedFreeCam()
{
	OnPressedFreeCam.ExecuteIfBound(true);
}

void APlayerCharacter::ReleasedFreeCam()
{
	OnPressedFreeCam.ExecuteIfBound(false);
}


/****************************************
	Test Function
*/
void APlayerCharacter::Test1()
{
	
}

void APlayerCharacter::Test2()
{
	/*if (GetWorld() && Cast<URLGameInstance>(GetWorld()->GetGameInstance()))
	{
		Cast<URLGameInstance>(GetWorld()->GetGameInstance())->TestPrintMap();
	}*/
	if (Cast<ARLPlayerController>(GetController()))
	{
		Cast<ARLPlayerController>(GetController())->ShowSelectItemWidget();
	}
}

void APlayerCharacter::Test3()
{
	FVector Loc = GetActorLocation();
	FRotator MovementRot = UKismetMathLibrary::MakeRotFromX(GetVelocity());
	MovementRot.Vector();

	Loc = Loc + MovementRot.Vector() * 500.f;
	TEnumAsByte<EMoveComponentAction::Type> Val = EMoveComponentAction::Move;
	FLatentActionInfo ActionInfo;
	ActionInfo.CallbackTarget = this;

	

	UKismetSystemLibrary::MoveComponentTo(RootComponent, Loc, GetActorRotation(), false, true, 0.2f, true, Val, ActionInfo);
	
	/*if (GetWorld() && Cast<URLGameInstance>(GetWorld()->GetGameInstance()))
	{
		Cast<URLGameInstance>(GetWorld()->GetGameInstance())->RequestMoveNextStage();
	}*/
}

/****************************************/
	



void APlayerCharacter::Interact()
{
	
}


void APlayerCharacter::Attack()
{
	Super::Attack();

	if (PlayerCombatComp)
	{
		PlayerCombatComp->ReadyToFire(true);
	}
}

void APlayerCharacter::AttackReleased()
{
	if (PlayerCombatComp)
	{
		PlayerCombatComp->ReadyToFire(false);
	}
}

void APlayerCharacter::StopFire()
{
	AttackReleased();
}

void APlayerCharacter::GetElementFromItem(EElement Element)
{
	if (ManagerComponent)
	{
		ManagerComponent->ApplyPlayerElement(Element);
	}
}

void APlayerCharacter::IncreaseMovementSpeed()
{
	if (GetMovementComponent())
	{
		GetCharacterMovement()->MaxWalkSpeed = 1100.f;
	}
}

void APlayerCharacter::DecreaseMovementSpeed()
{
	if (GetMovementComponent())
	{
		GetCharacterMovement()->MaxWalkSpeed = 700.f;
	}
}

void APlayerCharacter::HealByHit()
{
	if (ManagerComponent)
	{
		ManagerComponent->Heal(7.5f);
	}
}

void APlayerCharacter::HealByItem()
{
	if (ManagerComponent)
	{
		ManagerComponent->Heal(30.f);
	}
	ARLGameStateBase* GSB = Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this));
	if (GSB)
	{
		GSB->AteHealThisCell();
	}
}

FItemManager APlayerCharacter::GetItemManager() const
{
	if (ItemComp)
	{
		return ItemComp->GetItemManager();
	}
	return FItemManager();
}

void APlayerCharacter::RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem)
{
	if (ItemComp)
	{
		return ItemComp->ItemSwap(OldItem, NewItem);
	}
}

void APlayerCharacter::Recall()
{
	if (GetWorld() && Cast<URLGameInstance>(GetWorld()->GetGameInstance()))
	{
		Cast<URLGameInstance>(GetWorld()->GetGameInstance())->ReadyToRecall();
	}
}