#pragma once
#ifndef BLOCKS_H
#define BLOCKS_H

#include <cstdint>

struct Block
{
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

	std::uint16_t type = 0;
	std::uint16_t variation = 0;

	void sanatize()
	{
		if (type >= BLOCK_COUNT)
		{
			type = 0;
		}
	}
};

struct Wall
{
	enum
	{
		air = 0,
		dirtWall,
		grassWall,
		stoneWall,
		woodWall,
		sandStoneWall,
		brickWall,
		glassWall,
		copperBlockWall,
		silverBlockWall,
		goldBlockWall,
		snowWall,
		sandWall,
		stoneBricksWall,
		rubyBlockWall,
		heroglyphWall,
		blueRubyWall,
		plankedWall,
		snowBrickWall,
		boneBrickWall,

		BLOCKS_COUNT,
	};

	std::uint16_t variation = 0;
	std::uint16_t type = 0;
};

#endif
