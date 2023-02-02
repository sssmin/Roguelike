#pragma once


enum class ECellState : uint8 //�ʿ� ǥ���� �� ����
{
	NONE, 
	DEACTIVE,
	NORMAL,
	BONUS,
	DONTFINDBOSS,
	DISCOVEREDBOSS,
	INPLAYER,
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
