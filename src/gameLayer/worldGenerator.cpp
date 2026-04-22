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
	auto sandBiome = Biome{
		Block::sand,
		0,
		Block::sandStone,
		0,
		Wall::sand,
		0,
		static_cast<int>(w * 0.1f),
		static_cast<int>(w * 0.3f),
		50,
		static_cast<int>(h * 0.5f)
	};
	//createBiome(gameMap, { Block::sand, 0 }, { Wall::sand, 0 }, Vector2{ 0.1f, 0.5f }, { 0.2f, 0.4f });
	gameMap.desertStart = sandBiome.widthStart;
	gameMap.desertEnd = sandBiome.widthEnd;
	createBiome(gameMap, sandBiome);

	// Snow Biome

	auto snowBiome = Biome{
		Block::snow,
		0,
		Block::ice,
		0,
		Wall::snow,
		0,
		static_cast<int>(w * 0.6f),
		static_cast<int>(w * 0.9f),
		50,
		static_cast<int>(h * 0.7f),
	};
	createBiome(gameMap, snowBiome);

	//createWorldLayer(gameMap, Block{ Block::ice, 0 },  { 50, h * 0.4f }, { 4, 0.01f });

	// Perlin Worm
	
	generatePerlin(gameMap, Block{Block::air, 0});
	//generateOre(gameMap, Block{Block::copper, 0}, Wall{Wall::copper, 0}, 8, 0.1f, 5);
	/*
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
	*/
	for (int i = 0; i < gameMap.w * getRandomFloat(rng, 0.3f, 0.8f); i++)
	{
		float x = getRandomInt(rng, 10, w - 10);
		float y = getRandomInt(rng, 51, h - 10);

		int wormLength = getRandomInt(rng, getRandomInt(rng, 100, w / 4), getRandomInt(rng, 100, h / 4));


		if (getRandomChance(rng, 0.5f))
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
void generateWorm(GameMap& gameMap, float x, float y, int length, float radius, bool isBlock, Block block)
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
							b->type = block.type;
						}
					}
					else
					{

						if (digY < 200) { continue; }

						auto w = gameMap.getWallSafe(digX, digY);
						if (w)
						{
							w->type = block.type;
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

void generatePerlin(GameMap &gameMap, Block block, Vector2 worm, float chance)
{
	auto rng = gameMap.rng;
	auto w = gameMap.w;
	auto h = gameMap.w;

	for (int i = 0; i < gameMap.w * getRandomFloat(rng, 0.3f, 0.8f); i++)
	{
		float x = getRandomInt(rng, 10, w - 10);
		float y = getRandomInt(rng, 50, h - 10);

		auto wormLength = getRandomInt(rng, getRandomInt(rng, 100, h / 4), getRandomInt(rng, 100, h / 4));

		if (getRandomChance(rng, chance))
		{
			generateWorm(gameMap, x, y, wormLength, 5, true, block);
		}
	}
}

void generateOre(GameMap& gameMap, Block ore, Wall wall, int octaves, float frequency, float radius)
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
			if (getCaveNoise(x, y) < 0.7) // Percent of World
			{
				auto b = gameMap.getBlockUnsave(x, y);
				b.type = ore.type;
				b.variation = ore.variation;

				auto w = gameMap.getWallUnsave(x, y);
				w.type = wall.type;
				w.variation = wall.variation;
			}
		}
	}
	FastNoiseSIMD::FreeNoiseSet(caveNoise);

}

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
	for (int x = biome.widthStart; x < biome.widthEnd; x++)
	{
		int width = (biome.widthEnd - biome.widthStart) + noise[x] * biome.widthStart;
		int height = (biome.heightEnd - biome.heightStart) + noise[x] * biome.heightStart;
		for (int y = biome.heightStart; y < biome.heightEnd; y++)
		{
			/*
			// Biome Information Value
			int biomeMid = (biome.widthStart, biome.widthEnd) / 2;
			int biomeHalfWidth = (biome.widthEnd - biome.widthStart) / 2;
			int distanceFromBiomeMid = std::abs(x - biomeMid);

			float biomeDistance = 1 - distanceFromBiomeMid / static_cast<float>(biomeHalfWidth);

			int biomeStoneStart = 10 + 150;// 150 is Stone height
			int biomeStoneDepth = 20 + 150;

			int triangleStoneY = biomeStoneStart + biomeDistance * biomeStoneDepth;
			*/

			Block& b = gameMap.getBlockUnsave(x, y);
			Wall& w = gameMap.getWallUnsave(x, y);

			if (b.type == Block::air) { continue;}

			if (b.type != Block::air)
			{
				if (y <= height + 1 && hasGrass)// Grass
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

			b.variation = getRandomInt(rng, 0, 3);
			w.variation = getRandomInt(rng, 0, 3);

		}
	}
	gameMap.biomes.push_back(biome);
	FastNoiseSIMD::FreeNoiseSet(noise);
};
void createBiome2(GameMap& gameMap, Biome biome, bool hasGrass)
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
	Vector2 center = Vector2{ static_cast<float>(biome.widthEnd - biome.widthStart), static_cast<float>(biome.heightEnd - biome.heightStart) };
	float count = biome.widthEnd - biome.widthStart;



	for (int i = 0; i < count; i++)
	{
		float angle = (2 * PI / count) * i;

		float x = center.x + count * cos(angle);
		float y = center.y + count * sin(angle);

		Block& b = gameMap.getBlockUnsave(x, y);
		Wall& w = gameMap.getWallUnsave(x, y);

		if (b.type == Block::air) { continue; }

		if (b.type != Block::air)
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

		b.variation = getRandomInt(rng, 0, 3);
		w.variation = getRandomInt(rng, 0, 3);

	}

	gameMap.biomes.push_back(biome);
	FastNoiseSIMD::FreeNoiseSet(noise);
};