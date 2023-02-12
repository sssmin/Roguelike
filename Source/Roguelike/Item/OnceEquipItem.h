// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "OnceEquipItem.generated.h"


UCLASS()
class ROGUELIKE_API AOnceEquipItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	//아이템 기능들 구현->매니저컴포넌트에서 가진 아이템에 대한 액터를(이 액터)참조, 함수 참조
	
	
private:
	//EOnceEquipItem OnceEquipItemType;

public:
	//void SetEquipItemType(EOnceEquipItem EquipItemType) { OnceEquipItemType = EquipItemType;  }
};
