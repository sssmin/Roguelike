#pragma once

enum class EElement
{
	NONE,
	FIRE,
	WATER,
	EARTH,
	DARKNESS,
	LIGHT,

	MAX
};

struct FManage
{
	FManage() : MaxHP(100), CurrentHP(100), ATK(5), Element(EElement::NONE) {}
	FManage(int32 InMaxHP, int32 InATK, EElement InElement) :
		MaxHP(InMaxHP), CurrentHP(InMaxHP), ATK(InATK), Element(InElement) {}

	int32 MaxHP;
	int32 CurrentHP;

	int32 ATK;

	EElement Element;
};

