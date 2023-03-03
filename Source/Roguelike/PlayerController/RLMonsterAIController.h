// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "RLMonsterAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
class AMonsterCharacter;
class UHPBarWidget;
class UBossHPBarWidget;
class UAISenseConfig_Sight;
class UBehaviorTree;

UCLASS()
class ROGUELIKE_API ARLMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	ARLMonsterAIController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	void OnSkillEnd();
	UFUNCTION()
	void OnPercptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	void SetBehaviorTree(UBehaviorTree* BT);
	void SetTarget(AActor* Target);
	
private:
	UFUNCTION()
	void RunBT();
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BBComp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BTComp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;
	UPROPERTY()
	AMonsterCharacter* Monster;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UHPBarWidget> HPBarWidgetClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBossHPBarWidget> BossHPBarWidgetClass;
	UPROPERTY()
	UHPBarWidget* HPBarWidget;
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	
	float AISightRadius;
	float AISightAge;
	float AILoseSightRadius;
	float AIFieldOfView;
	
	
};
