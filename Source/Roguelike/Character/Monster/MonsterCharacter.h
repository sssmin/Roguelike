// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/BaseCharacter.h"
#include "Roguelike/Type/MonsterInterface.h"
#include "Roguelike/Actor/MeteorActor.h"
#include "MonsterCharacter.generated.h"

enum class EMonsterType : uint8
{
	NONE,
	NORMAL,
	ELITE,
	BOSS,

	MAX
};

class UWidgetComponent;
class UHPBarWidget;
class UMonsterCombatComponent;
class AMeteorActor;

UCLASS()
class ROGUELIKE_API AMonsterCharacter : public ABaseCharacter, public IMonsterInterface
{
	GENERATED_BODY()
	
public:
	AMonsterCharacter();
	virtual void BeginPlay() override;
	void SetHPBarWidget(const TSubclassOf<UHPBarWidget>& Widget);
	virtual void Attack() override;
	virtual void SpecialAttack(AActor* Target);

protected:
	void FireIn3Parts();
	void FireIn8Parts();
	void FireOneToTwo();
	void Teleport();
	void Meteor(const TSubclassOf<AMeteorActor>& Actor, AActor* Target);
	

private:
	EMonsterType MonsterType;

	UPROPERTY(EditInstanceOnly, Meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HPBarWidgetComp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UMonsterCombatComponent* MonsterCombatComp;


public:
	void SetMonsterType(EMonsterType Type) { MonsterType = Type; }
	EMonsterType GetMonsterType() const { return MonsterType; }
};
