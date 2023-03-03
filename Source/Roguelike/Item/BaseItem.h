// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"


class UParticleSystem;
class USphereComponent;

UCLASS()
class ROGUELIKE_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseItem();
	virtual void BeginPlay() override;
	
protected:
	UFUNCTION()
	virtual void ItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ItemParticle; //Element �������� ��
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* ItemSphere;


};
