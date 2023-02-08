#pragma once


enum class ECellState : uint8 //�ʿ� ǥ���� �� ����
{
	NONE, 
	DEACTIVE,
	NORMAL,
	BONUS,
	DONT_FIND_BOSS,
	DISCOVERED_BOSS,
	IN_PLAYER,
	CLEAR,

	MAX
};

enum class ECellType : uint8 //�� Ÿ��
{
	NONE,
	BONUS,
	MOBS,
	BOSS,
	START,

	MAX
};

