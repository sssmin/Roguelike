// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementItem.h"
#include "Kismet/GameplayStatics.h"
#include "Roguelike/Character/PlayerCharacter.h"

AElementItem::AElementItem()
{
	
}

void AElementItem::BeginPlay()
{

	Super::BeginPlay();
}



void AElementItem::SetParticle()
{
	FString ElementParticleTablePath = FString(TEXT("/Game/DataTable/ElementParticle"));
	UDataTable* ElementParticleTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ElementParticleTablePath));
	if (ElementParticleTableObject)
	{
		FElementParticleTable* Row = nullptr;
		Row = ElementParticleTableObject->FindRow<FElementParticleTable>(FName(FString::FromInt(static_cast<int32>(Element))), TEXT(""));
		if (Row)
		{
			ItemParticle = Row->Particle;
		}
	}

	if (ItemParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(
			ItemParticle,
			RootComponent,
			NAME_None,
			GetActorLocation(),
			GetActorRotation());
	}
}

void AElementItem::ItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::ItemInteract(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (Cast<APlayerCharacter>(OtherActor))
	{
		Cast<APlayerCharacter>(OtherActor)->GetElementFromItem(Element);
		Destroy();
	}
}