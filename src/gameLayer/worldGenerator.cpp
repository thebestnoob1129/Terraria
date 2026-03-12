#include "worldGenerator.h"
#include "randomStuff.h"
#include "raymath.h"

void generateWorld(GameMap& gameMap, unsigned int seed)
{
	
	const int w = 2000;
	const int h = 1000;

	gameMap.create(w, h);

	int stoneLayer = static_cast<int>(h * 0.5);
	int dirtLayer = static_cast<int>(h*0.4);

	std::ranlux24_base rng(seed);

	// 
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			
			Block b;
			Wall w;

			int sinX = x;
			int sinY = y;

			if ( y < h - (dirtLayer + stoneLayer))
			{
				// air
			}else if ( y == h - (dirtLayer + stoneLayer)) // Grass
			{
				//w.type = Wall::glassWall;
				b.type = Block::grassBlock;
			}
			else if (y < h -stoneLayer) // Dirt
			{
				//w.type = Wall::dirtWall;
				b.type = Block::dirt;
			}
			else if (y < h - dirtLayer)
			{
				b.type = Block::ice;
				sinY = (sin(x) * y);
			}
			else // Stone
			{
				//w.type = Wall::stoneWall;
				b.type = Block::stone;
			}

			sinY = Clamp(sinY, h - (dirtLayer + stoneLayer), h);

			gameMap.getBlockUnsave(x, sinY) = b;
			gameMap.getWallUnsave(x, y) = w;

		}
	}
}

/*
 *gameData.gameMap.create(30, 30);

	gameData.gameMap.getBlockUnsave(0, 0).type = Block::ice;
	gameData.gameMap.getWallUnsave(1, 0).type = Wall::brickWall;

	// Draw Ground
	for (int y = 0; y < gameData.gameMap.h; y++)
	{
		for (int x = 0; x < gameData.gameMap.w; x++)
		{
			//Tree(4 + x, 15, 8);

			if (y == 19)
			{
				gameData.gameMap.getBlockUnsave(x, y).type = Block::grass;
			}
			if (y > 20 && y <= 25)
			{
				gameData.gameMap.getBlockUnsave(x, y).type = Block::dirt;
			}
			if (y > 25)
			{
				gameData.gameMap.getBlockUnsave(x, y).type = Block::stone;
			}
		}
		
	}
	*/