#pragma once
#ifndef BLOCKS_H
#define BLOCKS_H

#include <cstdint>

struct Block
{
	std::uint16_t type = 0;
	std::uint16_t variation = 0;

	void sanatize()
	{
		if (type >= BLOCK_COUNT)
		{
			type = 0;
		}
	}

	bool isCollidable()
	{
		switch (type)
		{
		case air:
		case grass:
		case leaves:
		case platform:
		case workBench:
		case painting:
		case sappling:
		case door:
		case jar:
		case table:
		case wordrobe:
		case bookShelf:
		case icePlatform:
		case iceTable:
		case iceWordrobe:
		case iceBookShelf:
		case sandPlatform:
		case sandTable:
		case sandWordrobe:
		case sandBookShelf:
		case bonePlatform:
		case boneBench:
		case boneWordrobe:
		case boneBookShelf:
		case woodLog:
			return false;
		default:
			return true;

		}
	}

	enum
	{
		air = 0,
		dirt,
		grassBlock,
		stone,
		grass,
		sand,
		sandRuby,
		sandStone,
		woodPlank,
		stoneBricks,
		clay,
		woodLog,
		leaves,
		copper,
		iron,
		gold,
		copperBlock,
		ironBlock,
		goldBlock,
		bricks,
		snow,
		ice,
		rubyBlock,
		platform,
		workBench,
		glass,
		furnace,
		painting,
		sappling,
		snowBlueRuby,
		blueRubyBlock,
		door,
		jar,
		table,
		wordrobe,
		bookShelf,
		snowBricks,
		iceTable,
		iceWordrobe,
		iceBookShelf,
		icePlatform,
		sandTable,
		sandWordrobe,
		sandBookShelf,
		sandPlatform,
		woodenChest,
		iceChest,
		sandChest,
		boneChest,
		boneBricks,
		boneBench,
		boneWordrobe,
		boneBookShelf,
		bonePlatform,

		treeLog,
		treeLeafLog,
		treeLeafLogRight,
		treeLeafLogLeft,
		treeLogBase,
		treeLogLeaf,
		treeLogChopped,

		BLOCK_COUNT,
	};

};
struct Wall
{
	std::uint16_t type = 0;
	std::uint16_t variation = 0;

	void sanatize()
	{
		if (type >= BLOCK_COUNT)
		{
			type = 0;
		}
	}
	enum
	{
		air = 0,
		dirt,
		grassBlock,
		stone,
		grass,
		sand,
		sandRuby,
		sandStone,
		woodPlank,
		stoneBricks,
		clay,
		woodLog,
		leaves,
		copper,
		iron,
		gold,
		copperBlock,
		ironBlock,
		goldBlock,
		bricks,
		snow,
		ice,
		rubyBlock,
		platform,
		workBench,
		glass,
		furnace,
		painting,
		sappling,
		snowBlueRuby,
		blueRubyBlock,
		door,
		jar,
		table,
		wordrobe,
		bookShelf,
		snowBricks,
		iceTable,
		iceWordrobe,
		iceBookShelf,
		icePlatform,
		sandTable,
		sandWordrobe,
		sandBookShelf,
		sandPlatform,
		woodenChest,
		iceChest,
		sandChest,
		boneChest,
		boneBricks,
		boneBench,
		boneWordrobe,
		boneBookShelf,
		bonePlatform,

		treeLog,
		treeLeafLog,
		treeLeafLogRight,
		treeLeafLogLeft,
		treeLogBase,
		treeLogLeaf,
		treeLogChopped,

		BLOCK_COUNT,
	};

};
#endif
