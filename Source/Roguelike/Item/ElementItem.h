// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Engine/DataTable.h"
#include "Roguelike/Type/StatManage.h"
#include "ElementItem.generated.h"

class UParticleSystem;

USTRUCT(BlueprintType)
struct FElementParticleTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* Particle;
};


UCLASS()
class ROGUELIKE_API AElementItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AElementItem();
	virtual void BeginPlay() override;
	void SetParticle();
protected:
	virtual void ItemInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess ="true"))
	EElement Element;
	
public:
	void SetElement(EElement InElement) { Element = InElement; }
	
	
};
