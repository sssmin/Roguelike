#pragma once


enum class ECellState : uint8 //∏ ø° «•Ω√«“ ºø ªÛ≈¬
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

enum class ECellType : uint8 //ºø ≈∏¿‘
{
	None,
	Bonus,
	Mobs,
	Boss,
	Start,

	MAX
};

