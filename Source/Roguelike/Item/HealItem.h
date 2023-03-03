// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealItem.generated.h"

class UParticleSystemComponent;

UCLASS()
class ROGUELIKE_API AHealItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AHealItem();
	virtual void BeginPlay() override;
	
protected:
	virtual void ItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void Destroyed() override;
	
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HealedParticle;
	UPROPERTY()
	UParticleSystemComponent* HealItemParticleComp;
	
};
