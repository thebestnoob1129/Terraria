#include "worldGenerator.h"

void generateWorld(GameMap& gameMap, unsigned int seed, int w, int h)
{
	gameMap.create(w, h);

	w = std::clamp(w, 100, 10000);
	h = std::clamp(h, 100, 10000);

	std::ranlux24_base rng(seed++);

	float* mountainNoise = generateNoise(seed++, FastNoiseSIMD::SimplexFractal, 4, 0.03f, 0, w);
	float* plainsNoise = generateNoise(seed++, FastNoiseSIMD::SimplexFractal, 8, 0.01f, 0, w);
	float* caveNoise = generateNoise(seed++, FastNoiseSIMD::ValueFractal, 3, 0.02f, 0, h, w, 1);

	//convert from [-1 1 to [0 1]
	for (int i = 0; i < w; i++)
	{
		mountainNoise[i] = (mountainNoise[i] + 1) / 2;
		plainsNoise[i] = (plainsNoise[i] + 1) / 2;
		// stoneNoise[i] = std::pow(stoneNoise[i], 2); // Steeper Mountains
	}

	auto createLayer = [&](Block ground, Wall wall, Vector2 size, Vector2 hills)
		{
			// Fill Space with Block And Wall within diameter
			WorldLayer newLayer;
			std::ranlux24_base rng(gameMap.seed++);

			// Create Noise Map
			float* noise = generateNoise(gameMap.seed++, FastNoiseSIMD::SimplexFractal, hills.x, hills.y, 0.25f, gameMap.w);
			for (int i = 0; i < gameMap.w; i++)
			{
				noise[i] = Lerp((plainsNoise[i] + 1) / 2, std::pow(mountainNoise[i], 2), noise[i]);
			}


			// Generate World Data
			for (int i = 0; i < gameMap.w; i++)
			{
				int height = size.y + noise[i] * size.x;
				for (int y = 0; y < gameMap.h; y++)
				{
					Block& b = gameMap.getBlockUnsave(i, y);
					Wall& w = gameMap.getWallUnsave(i, y);
					if (y > height)
					{
						b.type = ground.type;
						w.type = wall.type;
					}
				}
			}

			gameMap.layerData.push_back(newLayer);
			FastNoiseSIMD::FreeNoiseSet(noise);
			return newLayer;
		};

	auto createBiome = [&](Block ground, Wall wall, Vector2 width, Vector2 height)
		{
			// Vector2 is percentage of world

			int biomeStart = gameMap.w * width.x;
			int biomeEnd = gameMap.w * width.y;

			int biomeHeightStart = gameMap.h * height.x;
			int biomeHeightEnd = gameMap.h * height.y;

			int biomeMid = (biomeStart, biomeEnd) / 2;
			int biomeHalfWidth = (biomeEnd - biomeStart) / 2;

			float* noise = generateNoise(gameMap.seed++, FastNoiseSIMD::Cellular, 4, 0.1f, 0, gameMap.w);

			for (int i = 0; i < gameMap.w; i++)
			{
				noise[i] = (noise[i] + 1) / 2; // Convert from [-1, 1] to [0, 1]
			}

			for (int x = biomeStart; x < biomeEnd; x++)
			{
				for (int y = biomeHeightStart; y < biomeHeightEnd; y++)
				{
					int newX = x + getRandomInt(rng, -20, 20);

					Block& b = gameMap.getBlockUnsave(newX, y);
					Wall& w = gameMap.getWallUnsave(newX, y);
					b.type = ground.type;
					w.type = wall.type;

					if (y > biomeHeightStart && y < biomeHeightEnd)
					{
						b.type = Block::sand;
					}

				}
			}

			// Check all objects in biome area and apply biome if in biome

			FastNoiseSIMD::FreeNoiseSet(noise);
			return true;
		};

	// Layers

	WorldLayer dirtLayer = createLayer(Block{ Block::dirt, 0 }, Wall{ Wall::dirtWall, 0 }, { 50, h * 0.2f }, { 8, 0.01f });
	WorldLayer stoneLayer = createLayer(Block{ Block::stone, 0 }, Wall{ Wall::stoneWall, 0 }, { 50, h * 0.4f }, { 4, 0.01f });
	WorldLayer iceLayer = createLayer(Block{ Block::ice, 0 }, Wall{ Wall::snowWall, 0 }, { 50, h * 0.6f }, { 4, 0.01f });

	// Biomes

	createBiome({ Block::sand, 0 }, { Wall::sandWall, 0 }, Vector2{ 0.2f, 0.4f }, { 0.1f, 0.4f });

	// Caves

	//generateCave(gameMap, seed++, 3, 0.02f);

	// Perlin Worm
	for (int i = 0; i < 100; i++)
	{
		float x = getRandomInt(rng, 10, w - 10);
		float y = getRandomInt(rng, 51, h - 10);

		int wormLength = getRandomInt(rng, getRandomInt(rng, 100, w / 4), getRandomInt(rng, 100, h / 4));


		if (getRandomChance(rng, 0.8f))
		{
			generateWorm(gameMap, x, y, wormLength);
		}
	}

	// Structures

	FastNoiseSIMD::FreeNoiseSet(plainsNoise);
	FastNoiseSIMD::FreeNoiseSet(mountainNoise);
	FastNoiseSIMD::FreeNoiseSet(caveNoise);
}
/*
void _generateWorld(GameMap& gameMap, unsigned int seed, int w, int h)
{
	gameMap.create(w, h);

	std::ranlux24_base rng(seed++);

	int snowStart = getRandomInt(rng, 10, w - 210);
	int snowEnd = snowStart + 100 + getRandomInt(rng, 0, 100);
	snowEnd = std::min<int>(w, snowEnd);
	
	Structure treeStructure;
	loadBlockDataFromFile(treeStructure.mapData, treeStructure.w, treeStructure.h, RESOURCES_PATH "structure/tree.bin");

	for (int x = 0; x < w; x++)
	{
		
		bool inSnow = (x > snowStart && x <= snowEnd);

		int dirtType = Block::dirt;
		int grassType = Block::grassBlock;
		int stoneType = Block::stone;
		
		if (inSnow)
		{
			dirtType = Block::snow;
			grassType = Block::snow;
			stoneType = Block::ice;
		}
		// for each layer

		// get the last layer
		
		// subtract the height

		for (int y = 0; y < h; y++)
		{
			
			Block b;
			Wall w;

			// if In Biome
			
			if (inSnow)
			{
				int snowMid = (snowStart + snowEnd) / 2;
				int snowHalfWidth = (snowEnd - snowStart) / 2;
				int distanceFromMid = std::abs(x - snowMid);

				// 0 (edge) to 1 (center)
				float snowDistance = 1 - distanceFromMid / float(snowHalfWidth);

				int snowStoneStart = 10 + stoneHeight;
				int snowStoneDepth = 20 + stoneHeight; // Deepness of biome

				int biomeStoneY = snowStoneStart + snowDistance * snowStoneDepth;
				// Apply stone if below biome

				if (y > biomeStoneY)
				{
					b.type = Block::stone;
					w.type = Block::stone;
				}

			}

			// Generate Caves

			if (getCaveNoise(x, y) < 0.5 && getCaveNoise(x, y) > 0.3) // Percent of World
			{
				b.type = Block::air;
			}

			b.variation = getRandomInt(rng, 0, 3);
			w.variation = getRandomInt(rng, 0, 3);

			gameMap.getBlockUnsave(x, y) = b;
			gameMap.getWallUnsave(x, y) = w;
			
		}
	}
}
*/

/*
void generateTrees()
{
	for (int x = 0; x < 100; x++)
	{
		if (getRandomChance(rng, 0.04f)) // Chance To Spawn Tree Per Column
		{
			for (int y = 0; y < h; y++)
			{
				auto type = gameMap.getBlockUnsave(x, y).type;
				if (type == Block::air) continue;
				if (type != Block::grassBlock) continue;

				Vector2 spawnPos = { (float)x, (float)y };

				spawnPos.x -= treeStructure.w / 2; // Center Tree
				spawnPos.y -= treeStructure.h; // Spawn on top of ground

				treeStructure.pasteIntoMap(gameMap, spawnPos);
				x += 3; // Avoids Tree Overlapping
				break;
			}
		}
	}
}
*/
Structure generateStructure(GameMap& gameMap, Vector2 position, const char* filename)
{
	std::string location = RESOURCES_PATH "structures/" + std::string(filename);

	Structure newStructure;
	loadBlockDataFromFile(newStructure.mapData, newStructure.w, newStructure.h, location.c_str());

	return newStructure;

}

float* generateNoise(unsigned int seed, FastNoiseSIMD::NoiseType noise, unsigned int octaves, float frequency, float gain, int w, int h, int z)
{
	// seed = world seed
	// fractal = fractal type
	// smoothness = fractal octaves
	// height = frequency

	std::unique_ptr<FastNoiseSIMD> generator(FastNoiseSIMD::NewFastNoiseSIMD());
	generator->SetSeed(seed++);
	generator->SetNoiseType(noise);
	generator->SetFractalOctaves(octaves);
	generator->SetFractalGain(gain);
	generator->SetFrequency(frequency);

	float* genNoise = FastNoiseSIMD::GetEmptySet(w * h); // h = 1

	generator->FillNoiseSet(genNoise, 0, 0, 0, w, h, z);

	return genNoise;

}

void generateCave(GameMap &gameMap, unsigned int seed, int octaves, float frequency, float radius)
{
	int w = gameMap.w;
	int h = gameMap.h;

	float* caveNoise = generateNoise(seed, FastNoiseSIMD::ValueFractal, octaves, frequency, 0, h, w, 1);

	for (int i = 0; i < w * h; i++)
	{
		caveNoise[i] = (caveNoise[i] + 1) / 2;
	}

	auto getCaveNoise = [&](int x, int y)
		{

			return caveNoise[x + y * w];
		};

	for (int x = 0; gameMap.w; x++)
	{
		for (int y = 0; gameMap.h; y++)
		{
			if (getCaveNoise(x, y) < 0.5 && getCaveNoise(x, y) > 0.3) // Percent of World
			{
				auto b = gameMap.getBlockUnsave(x, y);
				b.type = Block::air;

				auto wall = gameMap.getWallUnsave(x, y);
				wall.type = Wall::air;
			}
		}
	}
	FastNoiseSIMD::FreeNoiseSet(caveNoise);

}

void generateWorm(GameMap& gameMap, float x, float y, int length, float radius)
{

	std::ranlux24_base rng(gameMap.seed);

	float dirX = getRandomFloat(rng, -1, 1);
	float dirY = getRandomFloat(rng, -1, 1);

	// distance to change direction
	int changeDirTime = getRandomInt(rng, 5, 20);

	radius = getRandomInt(rng, 5, 35);
	radius = getRandomFloat(rng, radius * 0.75f, radius * 1.25f);

	for (int j = 0; j < length; j++)
	{
		
		// Clear Circle around positon
		int newRadius = getRandomFloat(rng, 0.5f, radius);

		int intRadius = std::ceil(newRadius);
		for (int ox = -intRadius; ox <= intRadius; ox++)
		{
			for (int oy = -intRadius; oy <= intRadius; oy++)
			{
				float distSq = ox * ox + oy * oy;
				if (distSq <= radius * radius)
				{
					int digX = x + ox;
					int digY = y + oy;

					auto b = gameMap.getBlockSafe(digX, digY);
					if (b)
					{
						b->type = Block::air;
					}
				}
			}
		}

		// Change Direction
		changeDirTime--;
		if (changeDirTime <= 0)
		{
			changeDirTime = getRandomInt(rng, 5, 20);

			if (getRandomChance(rng, 0.7f))
			{
				float keepFactor = 0.8f;

				dirX = dirX * keepFactor + getRandomFloat(rng) * (1.f - keepFactor);
				dirY = dirY * keepFactor + getRandomFloat(rng) * (1.f - keepFactor);
			}
			else
			{
				float keepFactor = 0.2f;

				dirX = dirX * keepFactor + getRandomFloat(rng) * (1.f - keepFactor);
				dirY = dirY * keepFactor + getRandomFloat(rng) * (1.f - keepFactor);
			}

		}

		// Move forward
		x += dirX * 1.5f;
		y += dirY * 1.5f;


		// Random radius wobble
		radius += (getRandomFloat(rng, -0.2f, 0.2f));
		radius = std::clamp(radius, 2.1f, 100.0f);

	}

}
