#pragma once


enum class ECellState : uint8 //∏ ø° «•Ω√«“ ºø ªÛ≈¬
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

enum class ECellType : uint8 //ºø ≈∏¿‘
{
	NONE,
	BONUS,
	MOBS,
	BOSS,
	START,

	MAX
};
