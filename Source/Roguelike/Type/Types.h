#pragma once


enum class ECellState : uint8 //∏ ø° «•Ω√«“ ºø ªÛ≈¬
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

enum class ECellType : uint8 //ºø ≈∏¿‘
{
	NONE,
	BONUS,
	MOBS,
	BOSS,
	START,

	MAX
};

