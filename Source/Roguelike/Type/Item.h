#pragma once

enum class EItemType : uint8 //아이템 종류
{
	NONE,
	ELEMENT,
	STACK_ITEM,
	ONCE_EQUIP_ITEM,

	MAX
};

enum class EOnceEquipItem : uint8
{
	DEFAULT,
	RIST_RETURN,
	MULTI_SHOT,


	MAX
};

