#include "worldGenerator.h"

#include <iostream>
#include <raymath.h>

void generateWorld(GameMap& gameMap, int w, int h)
{
	gameMap.create(w, h);

	w = std::clamp(w, 100, 10000);
	h = std::clamp(h, 100, 10000);

	std::ranlux24_base rng (gameMap.seed);

	float* caveNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, 3, 0.02f, 0, h, w);

	// Dirt

	auto dirtLayer = WorldLayer{
		Block::dirt,
		0,
		Wall::dirt,
		0,
		50,
		static_cast<int>(h * 0.2f),
		8,
		0.03f
	};
	//createWorldLayer(gameMap, Block{ Block::dirt, 0 }, { 50, h * 0.2f }, { 8, 0.03f }, true);
	createWorldLayer(gameMap, dirtLayer, true);
	// Stone
	
	auto stoneLayer = WorldLayer{
		Block::stone,
		0,
		Wall::stone,
		0,
		150,
		static_cast<int>(h * 0.3f),
		4,
		0.01f
	};
	//createWorldLayer(gameMap, Block{ Block::stone, 0 },  { 50, h * 0.3f }, { 4, 0.01f });
	createWorldLayer(gameMap, stoneLayer);
	
	// Sand Biome

	Biome sandBiome = {
		Block::sand,
		0,
		Block::sandStone,
		0,
		Wall::sand,
		0,
		static_cast<int>(w * 0.1f),
		static_cast<int>(w * 0.3f),
		100,
		static_cast<int>(h * 0.2f)
	};
	//createBiome(gameMap, { Block::sand, 0 }, { Wall::sand, 0 }, Vector2{ 0.1f, 0.5f }, { 0.2f, 0.4f });
	gameMap.desertStart = w * 0.1f;
	gameMap.desertEnd = w * 0.3f;
	createBiome(gameMap, sandBiome);
	
	// Snow
	//createWorldLayer(gameMap, Block{ Block::ice, 0 },  { 50, h * 0.4f }, { 4, 0.01f });

	//
	//createWorldLayer(gameMap, Block{ Block::sand, 0 },  Vector2{ 50, h * 0.5f }, { 4, 0.01f });
	// Biomes

	//createWorldLayer(gameMap, Block{ Block::rubyBlock, 0 },  Vector2{ 50, h * 0.6f }, { 4, 0.01f });

	// Caves

	//generateCave(gameMap, seed++, 3, 0.02f);

	// Perlin Worm
	for (int i = 0; i < gameMap.w * getRandomFloat(rng, 0.3f, 0.8f); i++)
	{
		float x = getRandomInt(rng, 10, w - 10);
		float y = getRandomInt(rng, 51, h - 10);

		int wormLength = getRandomInt(rng, getRandomInt(rng, 100, w / 4), getRandomInt(rng, 100, h / 4));


		if (getRandomChance(rng, 0.8f))
		{
			generateWorm(gameMap, x, y, wormLength, 5);
		}
	}

	for (int i = 0; i < gameMap.w * getRandomFloat(rng, 0.3f, 0.8f); i++)
	{
		float x = getRandomInt(rng, 10, w - 10);
		float y = getRandomInt(rng, 51, h - 10);

		int wormLength = getRandomInt(rng, getRandomInt(rng, 100, w / 4), getRandomInt(rng, 100, h / 4));


		if (getRandomChance(rng, 0.8f))
		{
			generateWorm(gameMap, x, y, wormLength, 5, false);
		}
	}

	// Structures
	generateTrees(gameMap);

	FastNoiseSIMD::FreeNoiseSet(caveNoise);
}

Structure generateStructure(GameMap& gameMap, const char* filename)
{
	std::string location = RESOURCES_PATH "structures/" + std::string(filename);

	Structure newStructure;
	loadBlockDataFromFile(newStructure.mapData, newStructure.wallData, newStructure.w, newStructure.h, location.c_str());

	return newStructure;

}

static void generateTrees(GameMap& gameMap)
{
	Structure treeStructure = generateStructure(gameMap, "tree.bin");

	for (int x = 0; x < gameMap.w; x++)
	{
		if (getRandomChance(gameMap.rng, 0.3f)) // Chance To Spawn Tree Per Column
		{
			for (int y = 0; y < gameMap.h; y++)
			{
				auto type = gameMap.getBlockUnsave(x, y).type;
				if (type == Block::air) continue;
				if (type != Block::grassBlock) continue;

				Vector2 spawnPos = { (float)x, (float)y };

				spawnPos.x -= static_cast<float>(treeStructure.w / 2); // Center Tree
				spawnPos.y -= treeStructure.h; // Spawn on top of ground

				treeStructure.pasteIntoMap(gameMap, spawnPos);
				x += 3; // Avoids Tree Overlapping
				break;
			}
		}
	}
}

float* generateNoise(int seed, FastNoiseSIMD::NoiseType noise, unsigned int octaves, float frequency, float gain, int w, int h, int z)
{
	// seed = world seed
	// fractal = fractal type
	// smoothness = fractal octaves
	// height = frequency


	// Issue one of these factors are set to 0
	if (h <= 0) h = 1;
	if (w <= 0) w = 1;
	if (z <= 0) z = 1;


	std::unique_ptr<FastNoiseSIMD> generator(FastNoiseSIMD::NewFastNoiseSIMD());
	generator->SetNoiseType(noise);
	generator->SetSeed(seed++);
	generator->SetFractalOctaves(octaves);
	generator->SetFractalGain(gain);
	generator->SetFrequency(frequency);

	float* genNoise = FastNoiseSIMD::GetEmptySet(w * h); // h = 1
	generator->FillNoiseSet(genNoise, 0, 0, 0, w, h, z);

	return genNoise;

}

void generateCave(GameMap &gameMap, int octaves, float frequency, float radius)
{
	int w = gameMap.w;
	int h = gameMap.h;

	float* caveNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, octaves, frequency, 0, h, w, 1);

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
			if (getCaveNoise(x, y) < 0.5) // Percent of World
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

void generateWorm(GameMap& gameMap, float x, float y, int length, float radius, bool isBlock)
{

	std::ranlux24_base rng = gameMap.rng;

	float dirX = getRandomFloat(rng, -1, 1);
	float dirY = getRandomFloat(rng, -1, 1);

	// distance to change direction
	int changeDirTime = getRandomInt(rng, 5, 20);

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

					if (isBlock) {

						auto b = gameMap.getBlockSafe(digX, digY);
						if (b)
						{
							b->type = Block::air;
						}
					}
					else
					{

						if (digY < 200) { continue; }

						auto w = gameMap.getWallSafe(digX, digY);
						if (w)
						{
							w->type = Wall::air;
						}
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
/*
void createWorldLayer(GameMap &gameMap, Block ground, Vector2 size, Vector2 hills, bool hasGrass)
{
	// Seed 
	std::cout << "Creating World: " << gameMap.seed << "\n";

	float* mountainNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, 4, 0.03f, 0, gameMap.w);
	float* plainsNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, 8, 0.01f, 0, gameMap.w);
	
	for (int i = 0; i < gameMap.w; i++)
	{
		mountainNoise[i] = (mountainNoise[i] + 1) / 2;
		plainsNoise[i] = (plainsNoise[i] + 1) / 2;
		// stoneNoise[i] = std::pow(stoneNoise[i], 2); // Steeper Mountains
	}

	// Fill Space with Block And Wall within diameter
	WorldLayer newLayer;
	std::ranlux24_base rng = gameMap.rng;

	// Create Noise Map
	float* noise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, hills.x, hills.y, 0.25f, gameMap.w);
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
				if (y <= height + 1 && hasGrass)
					{
						Block& g = gameMap.getBlockUnsave(i, y - 1);
						g.type = Block::grass;

						b.type = Block::grassBlock;
						w.type = Block::grassBlock;
					}
					else
					{
						b.type = ground.type;
						w.type = ground.type;
					}
				}
			}
		}

		gameMap.worldLayers.push_back(newLayer);
	FastNoiseSIMD::FreeNoiseSet(noise);
};


void createBiome(GameMap &gameMap, Block ground, Wall wall, Vector2 width, Vector2 height, bool hasGrass)
{
		// Vector2 is percentage of world

	// Seed 
	std::cout << "Creating World: " << gameMap.seed << "\n";

	float* mountainNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, 4, 0.03f, 0, gameMap.w);
	float* plainsNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, 8, 0.01f, 0, gameMap.w);

	for (int i = 0; i < gameMap.w; i++)
	{
		mountainNoise[i] = (mountainNoise[i] + 1) / 2;
		plainsNoise[i] = (plainsNoise[i] + 1) / 2;
		// stoneNoise[i] = std::pow(stoneNoise[i], 2); // Steeper Mountains
	}

	// Fill Space with Block And Wall within diameter
	std::ranlux24_base rng = gameMap.rng;


	int biomeStart = gameMap.w * width.x;
	int biomeEnd = gameMap.w * width.y;

	int biomeHeightStart = gameMap.h * height.x;
	int biomeHeightEnd = gameMap.h * height.y;

	float* noise = generateNoise(gameMap.seed, FastNoiseSIMD::Cellular, 4, 0.1f, 0, gameMap.w);
	for (int i = 0; i < gameMap.w; i++){
		noise[i] = (noise[i] + 1) / 2; // Convert from [-1, 1] to [0, 1]
	}

	for (int x = biomeStart; x < biomeEnd; x++)
	{
		for (int y = biomeHeightStart; y < biomeHeightEnd; y++)
		{
			// Get Biome Information Values
			int biomeMid = (biomeStart, biomeEnd) / 2;
			int biomeHalfWidth = (biomeEnd - biomeStart) / 2;
			int distanceFromBiomeMid = std::abs(x - biomeMid);

			float biomeDistance = 1 - distanceFromBiomeMid / static_cast<float>(biomeHalfWidth);

			int biomeStoneStart = 10 + 150;// 150 is Stone height
			int biomeStoneDepth = 20 + 150;

			int triangleStoneY = biomeStoneStart + biomeDistance * biomeStoneDepth;

			// Place Biome Blocks

			Block& t = gameMap.getBlockUnsave(x, y);
			if (t.type == Block::air) { continue; }

			if (y > triangleStoneY)
			{
				Block& b = gameMap.getBlockUnsave(x, y);
				Wall& w = gameMap.getWallUnsave(x, y);
			}

			Block& b = gameMap.getBlockUnsave(x, y);
			Wall& w = gameMap.getWallUnsave(x, y);
			if (t.type != Block::air) {
				b.type = ground.type;
				w.type = wall.type;
			}
			
				if (y > biomeHeightStart && y < biomeHeightEnd)
				{
					b.type = Block::sand;
				}
				
		}

		// Check all objects in biome area and apply biome if in biome

		FastNoiseSIMD::FreeNoiseSet(noise);
	}
};
*/
void createWorldLayer(GameMap& gameMap, WorldLayer layer, bool hasGrass)
{
	// Seed 

	float* mountainNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, 4, 0.03f, 0, gameMap.w);
	float* plainsNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, 8, 0.01f, 0, gameMap.w);

	for (int i = 0; i < gameMap.w; i++)
	{
		mountainNoise[i] = (mountainNoise[i] + 1) / 2;
		plainsNoise[i] = (plainsNoise[i] + 1) / 2;
		// stoneNoise[i] = std::pow(stoneNoise[i], 2); // Steeper Mountains
	}

	// Fill Space with Block And Wall within diameter
	std::ranlux24_base rng = gameMap.rng;

	// Create Noise Map
	float* noise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, layer.smoothness, layer.frequency , 0.25f, gameMap.w);
	for (int i = 0; i < gameMap.w; i++)
	{
		noise[i] = Lerp((plainsNoise[i] + 1) / 2, std::pow(mountainNoise[i], 2), noise[i]);
	}


	// Generate World Data
	for (int i = 0; i < gameMap.w; i++)
	{
		int height = layer.heightEnd + noise[i] * layer.heightStart;
		for (int y = 0; y < gameMap.h; y++)
		{
			Block& b = gameMap.getBlockUnsave(i, y);
			Wall& w = gameMap.getWallUnsave(i, y);

			if (y > height)
			{
				if (y <= height + 1 && hasGrass)
				{
					Block& g = gameMap.getBlockUnsave(i, height);
					g.type = Block::grass;

					b.type = Block::grassBlock;
					w.type = Block::grassBlock;
				}
				else
				{
					b.type = layer.block.type;
					w.type = layer.wall.type;
				}
			}
		}
	}
	gameMap.worldLayers.push_back(layer);
	FastNoiseSIMD::FreeNoiseSet(noise);
};
void createBiome(GameMap& gameMap, Biome biome, bool hasGrass)
{
	// Seed 

	float* mountainNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, 4, 0.03f, 0, gameMap.w);
	float* plainsNoise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, 8, 0.01f, 0, gameMap.w);

	for (int i = 0; i < gameMap.w; i++)
	{
		mountainNoise[i] = (mountainNoise[i] + 1) / 2;
		plainsNoise[i] = (plainsNoise[i] + 1) / 2;
		// stoneNoise[i] = std::pow(stoneNoise[i], 2); // Steeper Mountains
	}

	// Fill Space with Block And Wall within diameter
	std::ranlux24_base rng = gameMap.rng;

	// Create Noise Map
	float* noise = generateNoise(gameMap.seed, FastNoiseSIMD::SimplexFractal, biome.smoothness, biome.frequency , 0.25f, gameMap.w);
	for (int i = 0; i < gameMap.w; i++)
	{
		noise[i] = Lerp((plainsNoise[i] + 1) / 2, std::pow(mountainNoise[i], 2), noise[i]);
	}


	// Generate World Data
	// not sure about biome width start & end
	for (int x = 0; x < gameMap.w; x++)
	{
		int width = (biome.widthEnd - biome.widthStart) + noise[x] * biome.widthStart;
		int height = (biome.heightEnd - biome.heightStart) + noise[x] * biome.heightStart;
		for (int y = 0; y < gameMap.h; y++)
		{
			// Biome Information Value
			int biomeMid = (biome.widthStart, biome.widthEnd) / 2;
			int biomeHalfWidth = (biome.widthEnd - biome.widthStart) / 2;
			int distanceFromBiomeMid = std::abs(x - biomeMid);

			float biomeDistance = 1 - distanceFromBiomeMid / static_cast<float>(biomeHalfWidth);

			int biomeStoneStart = 10 + 150;// 150 is Stone height
			int biomeStoneDepth = 20 + 150;

			int triangleStoneY = biomeStoneStart + biomeDistance * biomeStoneDepth;


			Block& b = gameMap.getBlockUnsave(x, y);
			Wall& w = gameMap.getWallUnsave(x, y);

			if (b.type == Block::air) { continue;}

			// if Within height bounds


			//if (y > biome.heightStart && biome.heightEnd > y && biome.widthStart < x && biome.widthEnd > x) {
				if (y > height && y < biome.heightEnd)
				{
					if (y <= biome.heightStart + 1 && hasGrass)// Grass
					{
						Block& g = gameMap.getBlockUnsave(x, y - 1);
						g.type = Block::grass;

						b.type = Block::grassBlock;
						w.type = Block::grassBlock;
					}
					else
					{
						if (b.type == Block::stone)
						{
							b.type = biome.stoneBlock.type;
						}
						else
						{
							b.type = biome.groundBlock.type;
						}
						w.type = biome.wall.type;
					}
				}
			//}
		}
	}
	gameMap.biomes.push_back(biome);
	FastNoiseSIMD::FreeNoiseSet(noise);
};