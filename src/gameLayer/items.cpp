#include "items.h"

#include <assetManager.h>
#include <helpers.h>

Texture getTextureForItemType(int itemType, AssetManager& assetManager)
{
	if (itemType < Block::BLOCK_COUNT)
	{
		return assetManager.textures;
	}
	else
	{
		return assetManager.items;
	}
}

Rectangle getTextureCoordinatesForItemType(int itemType)
{
	if (itemType < Block::BLOCK_COUNT)
	{
		return getTextureAtlas(itemType, 4, 32, 32);
	}
	else
	{
		return getTextureAtlas(itemType - Block::BLOCK_COUNT, 0, 32, 32);
	}
}

bool isItem(int type)
{
	return type >= Block::BLOCK_COUNT;
}

bool isBlock(int type)
{
	return !isItem(type);
}

// Example of Global Comparison Implementation
bool isThrowable(int type)
{
	switch (type)
	{
	case (Item::apple):
	case (Item::book):
	case (Item::shuriken):
		return true;
	default:
		return false;
	}
}
