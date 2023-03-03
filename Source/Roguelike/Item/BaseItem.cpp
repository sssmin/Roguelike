// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseItem.h"
#include "Components/SphereComponent.h"


ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphere"));
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemSphere)
	{
		ItemSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ItemInteract);
	}
	
}

void ABaseItem::ItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

