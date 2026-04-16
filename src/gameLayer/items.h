#pragma once
#ifndef ITEM_H
#define ITEM_H

#include <blocks.h>
#include <assetManager.h>

struct AssetManager;

bool isItem(int type);
bool isBlock(int type);
bool isThrowable(int type);

bool isTool(int type);
bool isWeapon(int type);
bool isPickaxe(int type);
bool isHammer(int type);

bool isArmor(int type);
bool isHelmet(int type);
bool isChestplate(int type);



struct Item
{

	enum
	{
		woodPickaxe = Block::BLOCK_COUNT + 1,
		woodAxe,
		woodHammer,
		stonePickaxe,
		stoneAxe,
		stoneHammer,
		copperPickaxe,
		copperAxe,
		copperHammer,
		ironPickaxe,
		ironAxe,
		ironHammer,
		goldPickaxe,
		goldAxe,
		goldHammer,
		slime,
		copperIngot,
		ironIngot,
		goldIngot,
		woodenSword,
		stoneSword,
		copperSword,
		ironSword,
		goldSword,
		ruby,
		copperSpear,
		ironSpear,
		goldSpear,
		blueRuby,
		iceSpear,
		healthPotion,
		apple,
		paper,
		book,
		bone,
		shuriken,
		copperDagger,
		ironDagger,
		iceDagger,
		copperChestPlate,
		copperHelmet,
		copperBoots,

		ironChestPlate,
		ironHelmet,
		ironBoots,

		goldChestPlate,
		goldHelmet,
		goldBoots,

		partyHat,
		sunGlasses,

		iceChestPlate,
		iceHelmet,
		iceBoots,

		LAST_ITEM

	};

	int type = 0;
	int count = 1;

	bool isItem()
	{
		return ::isItem(type);
	}

	bool isBlock()
	{
		return ::isBlock(type);
	}

};

Texture getTextureForItemType(int itemType, AssetManager& assetManager);

Rectangle getTextureCoordinatesForItemType(int itemType);

#endif
