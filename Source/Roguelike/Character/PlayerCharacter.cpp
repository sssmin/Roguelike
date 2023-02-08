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
#include "Roguelike/Type/Manage.h"

#include "Roguelike/Component/ManagerComponent.h" //Test

APlayerCharacter::APlayerCharacter()
{
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
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		SetActorLocation(GI->GetPlayerSpawnLoc());
	}
	if (GetWorld())
	{
		PC = GetWorld()->GetFirstPlayerController();
	}
	if (PC)
	{
		FInputModeGameAndUI InputModeData;
		InputModeData.SetHideCursorDuringCapture(false);
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
		PC->SetInputMode(InputModeData);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookAtCursor();
}

void APlayerCharacter::LookAtCursor()
{
	FVector Start = GetActorLocation();
	FVector Target;
	FHitResult Hit;
	if (PC)
	{
		PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			Target = Hit.ImpactPoint;
		}
	}
	LookRot = UKismetMathLibrary::FindLookAtRotation(Start, Target);
	SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("TestLevelMove", IE_Pressed, this, &ThisClass::TestKillMe);
	PlayerInputComponent->BindAction("TestIncreaseKillCount", IE_Pressed, this, &ThisClass::TestIncreaseKillCount);
	PlayerInputComponent->BindAction("TestPrintMap", IE_Pressed, this, &ThisClass::TestPrintMap);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ThisClass::Attack);

	PlayerInputComponent->BindAction("FreeCam", IE_Pressed, this, &ThisClass::PressedFreeCam);
	PlayerInputComponent->BindAction("FreeCam", IE_Released, this, &ThisClass::ReleasedFreeCam);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ThisClass::MoveRight);
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
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
void APlayerCharacter::TestIncreaseKillCount()
{
	ARLGameStateBase* GSB = Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this));
	if (GSB)
	{
		GSB->TestKillScored();
	}

}

void APlayerCharacter::TestKillMe()
{
	if (ManagerComp)
	{
		ManagerComp->TestHurt();
	}
}

void APlayerCharacter::TestPrintMap()
{
	if (GetWorld() && Cast<URLGameInstance>(GetWorld()->GetGameInstance()))
	{
		Cast<URLGameInstance>(GetWorld()->GetGameInstance())->TestPrintMap();
	}
}


void APlayerCharacter::Interact()
{
	
}


void APlayerCharacter::Attack()
{
	Super::Attack();
}

void APlayerCharacter::GetElementFromItem(int32 ConvertElement)
{
	if (ManagerComp)
	{
		ManagerComp->ApplyPlayerElement(ConvertElement);
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
	if (ManagerComp)
	{
		ManagerComp->Heal(7.5f);
	}
}

void APlayerCharacter::HealByItem()
{
	if (ManagerComp)
	{
		ManagerComp->Heal(30.f);
	}
}