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
		case Block::air:
		case Block::grass:
		case Block::leaves:
		case Block::platform:
		case Block::workBench:
		case Block::painting:
		case Block::sappling:
		case Block::door:
		case Block::jar:
		case Block::table:
		case Block::wordrobe:
		case Block::bookShelf:
		case Block::icePlatform:
		case Block::iceTable:
		case Block::iceWordrobe:
		case Block::iceBookShelf:
		case Block::sandPlatform:
		case Block::sandTable:
		case Block::sandWordrobe:
		case Block::sandBookShelf:
		case Block::bonePlatform:
		case Block::boneBench:
		case Block::boneWordrobe:
		case Block::boneBookShelf:
		case Block::woodLog:
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

bool isCollidable(int type);
#endif
