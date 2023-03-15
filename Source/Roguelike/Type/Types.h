#pragma once


enum class ECellState : uint8 //�ʿ� ǥ���� �� ����
{
	None, 
	Deactivate,
	Normal,
	Bonus,
	DontFindBoss,
	DiscoveredBoss,
	InPlayer,
	Clear,

	MAX
};

enum class ECellType : uint8 //�� Ÿ��
{
	None,
	Bonus,
	Mobs,
	Boss,
	Start,

	MAX
};

