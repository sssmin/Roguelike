// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Roguelike/Type/Item.h"
#include "BaseItem.generated.h"


class UParticleSystem;
class USphereComponent;

UCLASS()
class ROGUELIKE_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseItem();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void ItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ItemParticle; //Element ½ºÆùµÆÀ» ¶§

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* ItemSphere;

private:	
	EItemType ItemType;
	

public:
	void SetItemType(EItemType Type) { ItemType = Type; }
};
