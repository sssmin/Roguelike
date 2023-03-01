// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MonsterCharacter.h"
#include "MonsterSuicide.generated.h"

class UBoxComponent;

UCLASS()
class ROGUELIKE_API AMonsterSuicide : public AMonsterCharacter
{
	GENERATED_BODY()
public:
	AMonsterSuicide();
	virtual void BeginPlay() override;
	virtual void GiveBTToController() override;
	virtual void Destroyed() override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* SuicideBT;
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComp;
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
