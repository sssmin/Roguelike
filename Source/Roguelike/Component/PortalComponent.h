// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PortalComponent.generated.h"


class APortalActor;


USTRUCT()
struct FPortalInfo
{
	GENERATED_BODY()
	FPortalInfo() : Dir(0), Portal(nullptr) {}
	FPortalInfo(int32 InDir, APortalActor* InPortal) : Dir(InDir), Portal(InPortal) {}

	int32 Dir;
	UPROPERTY()
	APortalActor* Portal;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UPortalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPortalComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CreateSidePortal();
	void CreateCenterPortal();
	void CreatePrevBossPortal();
	void DestroyPortal();
	FVector GetArrowLocation(int32 Dir);
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:	
	TArray<FPortalInfo> Portals;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APortalActor> PortalActorClass;
	void SetLocationPotal();
	void CreatePortal(TArray<int32> Dirs);
	FVector CalcLocation(int32 Dir);
	UPROPERTY()
	APortalActor* PrevBossPortal;
public:
	
};
