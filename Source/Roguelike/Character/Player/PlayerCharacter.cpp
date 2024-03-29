// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLMainGameState.h"
#include "Roguelike/Type/ItemManage.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Component/ItemComponent.h"
#include "Roguelike/Component/PlayerCombatComponent.h"
#include "Roguelike/Interface/InteractInterface.h"
#include "Roguelike/Character/CharacterAnimInstance.h"
#include "Roguelike/Actor/DamageWidgetActor.h"
#include "Roguelike/Game/RLMainGameMode.h"
#include "Roguelike/Game/RLTutorialGameMode.h"
#include "Roguelike/Game/TutorialManager.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "Roguelike/Widget/DamageWidget.h"


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
	MaxDashDist = 500.f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputModeData);
		}
	}

	if (PlayerCombatComp)
	{
		PlayerCombatComp->GetItemManager.BindUObject(this, &ThisClass::GetItemManager);
		PlayerCombatComp->GetCombatManager.BindUObject(this, &ABaseCharacter::GetCombatManager);
	}
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
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ThisClass::Interact);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ThisClass::Dash);
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
	// FVector OutVector;
	// UGameplayStatics::SuggestProjectileVelocity_CustomArc(
	// 	this,
	// 	OutVector,
	// 	GetActorLocation(),
	// 	GetActorLocation() + GetActorForwardVector() * 1000.f,
	// 	GetWorld()->GetGravityZ()
	// );
	// FPredictProjectilePathParams Params(20.f, GetActorLocation(), OutVector, 15.f);
	// Params.DrawDebugTime = 15.f;
	// Params.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
	// Params.OverrideGravityZ = GetWorld()->GetGravityZ();
	// FPredictProjectilePathResult Result;
	// UGameplayStatics::PredictProjectilePath(
	// 	this,
	// 	Params,
	// 	Result
	// );
	ManagerComponent->HealByRate(100.f);
	ManagerComponent->UpdateCurrentAtk(100.f);
}

void APlayerCharacter::Test2()
{
	// if (GetWorld() && Cast<URLGameInstance>(GetWorld()->GetGameInstance()))
	// {
	// 	Cast<URLGameInstance>(GetWorld()->GetGameInstance())->TestPrintMap();
	// }
	if (Cast<ARLPlayerController>(GetController()))
	{
		Cast<ARLPlayerController>(GetController())->ShowSelectItemWidget(false);
	}
	// GetWorld()->GetTimerManager().SetTimer(TestTimerHandle, [this]()
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("Timer Call"));
	// 		}, 10.f, false);
}


void APlayerCharacter::Test3()
{
	ARLTutorialGameMode* GM = Cast<ARLTutorialGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->GetTutorialManager())
	{
		GM->GetTutorialManager()->SetNextTutorial();
	}
	
	
	/*if (GetWorld() && Cast<URLGameInstance>(GetWorld()->GetGameInstance()))
	{
		Cast<URLGameInstance>(GetWorld()->GetGameInstance())->RequestMoveNextStage();
	}*/
}

/****************************************/
	
void APlayerCharacter::Interact()
{
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + GetActorForwardVector() * 500.f;
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);
	if (HitResult.GetActor())
	{
		if (HitResult.GetActor()->Implements<UInteractInterface>())
		{
			Cast<IInteractInterface>(HitResult.GetActor())->Interact();
		}
	}
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

void APlayerCharacter::ShowNumWidget(float Damage, bool IsCritical, bool IsHeal, bool IsDodge)
{
	FActorSpawnParameters Params;
	FTransform SpawnTransform = GetActorTransform();
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Y += FMath::RandRange(-100.f, 100.f);
	SpawnTransform.SetLocation(SpawnLocation);
	
	if (GetDamageWidgetActorClass())
	{
		ADamageWidgetActor* DamageWidgetActor = GetWorld()->SpawnActor<ADamageWidgetActor>(GetDamageWidgetActorClass(), SpawnTransform, Params);
		if (DamageWidgetActor)
		{
			DamageWidgetActor->PlayNumWidget(true, IsCritical, Damage, IsHeal, IsDodge);
		}
	} 
}

void APlayerCharacter::ApplyMovementBuff() const
{
	if (ManagerComponent)
	{
		ManagerComponent->ApplyMovementBuff();
	}
}

void APlayerCharacter::SetStateIcon(EState State)
{
	ARLMainGameMode* MainGM = Cast<ARLMainGameMode>(UGameplayStatics::GetGameMode(this));
	ARLPlayerController* RLPC = Cast<ARLPlayerController>(PC);
	if (MainGM && RLPC)
	{
		if (UTexture2D* Image = MainGM->GetStateImage(State))
		{
			RLPC->SetStateIcon(State, Image);	
		}
	}
}

void APlayerCharacter::RemoveStateIcon(EState State)
{
	if (ARLPlayerController* RLPC = Cast<ARLPlayerController>(PC))
	{
		RLPC->RemoveStateIcon(State);
	}
}

void APlayerCharacter::FlickerStateIcon(EState State)
{
	if (ARLPlayerController* RLPC = Cast<ARLPlayerController>(PC))
	{
		RLPC->FlickerStateIcon(State);
	}
}

void APlayerCharacter::SetBuffIcon(EBuff Buff)
{
	ARLMainGameMode* MainGM = Cast<ARLMainGameMode>(UGameplayStatics::GetGameMode(this));
	ARLPlayerController* RLPC = Cast<ARLPlayerController>(PC);
	if (MainGM && RLPC)
	{
		UTexture2D* Image = MainGM->GetBuffImage(Buff);
		RLPC->SetBuffIcon(Buff, Image);
	}
}

void APlayerCharacter::RemoveBuffIcon(EBuff Buff)
{
	if (ARLPlayerController* RLPC = Cast<ARLPlayerController>(PC))
	{
		RLPC->RemoveBuffIcon(Buff);
	}
}

void APlayerCharacter::FlickerBuffIcon(EBuff Buff)
{
	if (ARLPlayerController* RLPC = Cast<ARLPlayerController>(PC))
	{
		RLPC->FlickerBuffIcon(Buff);
	}
}

void APlayerCharacter::GetElementFromItem(EElement Element) const
{
	if (ManagerComponent)
	{
		ManagerComponent->ApplyPlayerElement(Element);
	}
}

void APlayerCharacter::IncreaseMovementSpeed() const
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 1100.f;
	}
}

void APlayerCharacter::DecreaseMovementSpeed() const
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 700.f;
	}
}

void APlayerCharacter::HealByItem() const
{
	if (ManagerComponent)
	{
		ManagerComponent->HealByRate(30.f);
	}
	if (ARLMainGameState* GSB = Cast<ARLMainGameState>(UGameplayStatics::GetGameState(this)))
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

void APlayerCharacter::RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem) const
{
	if (ItemComp)
	{
		return ItemComp->ItemSwap(const_cast<UItemInfo*>(OldItem), const_cast<UItemInfo*>(NewItem));
	}
}

void APlayerCharacter::Recall()
{
	if (URLGameInstance* GI = URLGameInstance::GetRLGameInst(this))
	{
		GI->ReadyToRecall();
	}
}

void APlayerCharacter::Dash()
{
	if (GetVelocity().Size() <= 0) return;
	check(PlayerCombatComp);
	
	if (PlayerCombatComp->CanDash())
	{
		FVector Loc = GetActorLocation();
		const FRotator MovementRot = UKismetMathLibrary::MakeRotFromX(GetVelocity());
		float Dist = MaxDashDist;

		CheckToGo(Dist);
		
		Loc = Loc + MovementRot.Vector() * Dist;
		const TEnumAsByte<EMoveComponentAction::Type> Val = EMoveComponentAction::Move;
		FLatentActionInfo ActionInfo;
		ActionInfo.CallbackTarget = this;
		
		UKismetSystemLibrary::MoveComponentTo(
			RootComponent,
			Loc,
			GetActorRotation(),
			false,
			true,
			0.2f,
			true,
			Val,
			ActionInfo);
		
		PlayerCombatComp->DecCurrentDashChargeNum();
	}
}

void APlayerCharacter::CheckToGo(OUT float& Dist) const
{
	FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
	FVector PlayerLoc = GetActorLocation();
    GetWorld()->LineTraceSingleByChannel(
    	HitResult,
    	PlayerLoc + GetVelocity().GetSafeNormal(),
    	PlayerLoc + GetVelocity().GetSafeNormal() * MaxDashDist,
    	ECollisionChannel::ECC_Visibility,
    	Params
    	);

    if (HitResult.bBlockingHit)
    {
    	Dist = GetDistanceTo(PlayerLoc, HitResult.ImpactPoint) - 50.f;
    }
}

float APlayerCharacter::GetDistanceTo(FVector StandardLoc, FVector TargetLoc) const
{
	return FMath::Sqrt(FMath::Pow(StandardLoc.X - TargetLoc.X, 2) +
							FMath::Pow(StandardLoc.Y - TargetLoc.Y, 2) +
							FMath::Pow(StandardLoc.Z - TargetLoc.Z, 2));
}

void APlayerCharacter::OnSkillHit(AActor* Attacker, AActor* DamageCauser, const FCombatManager& EnemyManager, TSubclassOf<UDamageType> DamageType)
{
	if (ManagerComponent)
	{
		ManagerComponent->ReceiveDamage(EnemyManager, FItemManager(), Attacker, DamageCauser, DamageType);
	}
}

void APlayerCharacter::Dead()
{
	Super::Dead();

	if (GetMesh() && Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance())->SetIsDead((true));
	}
}
