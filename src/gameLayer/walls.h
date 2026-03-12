#pragma once
#ifndef WALLS_H
#define WALLS_H

#include <cstdint>

#include "randomStuff.h"

struct Wall
{
	enum
	{
		air = 0,
		dirtWall,
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
