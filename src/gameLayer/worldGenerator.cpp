#include "worldGenerator.h"

void generateWorld(GameMap& gameMap, unsigned int seed, int w, int h)
{
	gameMap.create(w, h);

	std::ranlux24_base rng(seed++);

#pragma region Layers
	WorldLayer dirtLayer = {
		.block = Block::dirt,
		.wall = Wall::dirtWall,
		.heightStart = -50,
		.heightEnd = 35,
		.smoothness = 8,
		.frequency = 0.01f
	};
	gameMap.layerData.push_back(dirtLayer);

	WorldLayer stoneLayer = {
		.block = Block::stone,
		.wall = Wall::stoneWall,
		.heightStart = static_cast<int>(h * 0.3f),
		.heightEnd = static_cast<int>(h * 0.4f),
		.smoothness = 4,
		.frequency = 0.01f
	};
	gameMap.layerData.push_back(stoneLayer);

	WorldLayer sandLayer = {
		.block = Block::sand,
		.wall = Wall::sandWall,
		.heightStart = static_cast<int>(h*0.5f),
		.heightEnd = static_cast<int>(h*0.6),
		.smoothness = 4,
		.frequency = 0.01f,
	};
	gameMap.layerData.push_back(sandLayer);

	// Layer Noise Generator
	float* dirtNoise = generateNoise(seed++, FastNoiseSIMD::SimplexFractal, dirtLayer.smoothness, dirtLayer.frequency, 0, w);
	float* stoneNoise = generateNoise(seed++, FastNoiseSIMD::SimplexFractal, stoneLayer.smoothness, stoneLayer.frequency, 0, w);
	float* sandNoise = generateNoise(seed++, FastNoiseSIMD::SimplexFractal, sandLayer.smoothness, sandLayer.frequency, 0, w);
	float* mountainNoise = generateNoise(seed++, FastNoiseSIMD::SimplexFractal,  4, 0.03f, 0, w);
	float* plainsNoise = generateNoise(seed++, FastNoiseSIMD::SimplexFractal, 8, 0.01f, 0, w);
	float* caveNoise = generateNoise(seed++, FastNoiseSIMD::ValueFractal, 3, 0.02f, 0, h, w, 1);

	//convert from [-1 1 to [0 1]
	for (int i = 0; i < w; i++)
	{
		mountainNoise[i] = (mountainNoise[i] + 1) / 2;
		plainsNoise[i] = (plainsNoise[i] + 1) / 2;
		
		//dirtNoise[i] = (dirtNoise[i] + 1) / 2;
		dirtNoise[i] = Lerp((plainsNoise[i] + 1) / 2, std::pow(mountainNoise[i], 2), dirtNoise[i]) ;
		//stoneNoise[i] = (stoneNoise[i] + 1) / 2;
		stoneNoise[i] = Lerp((plainsNoise[i] + 1) / 2, std::pow(mountainNoise[i], 2), stoneNoise[i]) ;

		sandNoise[i] = Lerp((plainsNoise[i] + 1) / 2, std::pow(mountainNoise[i], 2), sandNoise[i]) ;
		// stoneNoise[i] = std::pow(stoneNoise[i], 2); // Steeper Mountains
	}

	for (int i = 0; i < w*h; i++)
	{
		caveNoise[i] = (caveNoise[i] + 1) / 2;
	}

	auto getCaveNoise = [&](int x, int y)
	{
		return caveNoise[x + y * w];
	};

#pragma endregion


	int snowStart = getRandomInt(rng, 10, w - 210);
	int snowEnd = snowStart + 100 + getRandomInt(rng, 0, 100);
	snowEnd = std::min<int>(w, snowEnd);
	
	
	// 
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


		// Create Dynamic

		int sandHeight = sandLayer.heightStart + (sandLayer.heightEnd - sandLayer.heightStart) * sandNoise[x];
		int stoneHeight = sandHeight - ( stoneLayer.heightStart + (stoneLayer.heightEnd - stoneLayer.heightStart) * stoneNoise[x]);
		int dirtHeight =  stoneHeight - ( dirtLayer.heightStart + (dirtLayer.heightEnd - dirtLayer.heightStart) * dirtNoise[x]);
		// for each layer

		// get the last layer
		
		// subtract the height

		for (int y = 0; y < h; y++)
		{
			
			Block b;
			Wall w;

			if (y > dirtHeight)
			{
				b.type = dirtType;
				w.type = dirtType;
			}

			if (y == dirtHeight)
			{
				b.type = grassType;
				w.type = grassType;
			}
			
			if (y >= stoneHeight)
			{
				b.type = stoneType;
				w.type = stoneType;
			}

			if (y >= sandHeight)
			{
				b.type = sandLayer.block.type;
				w.type = sandLayer.wall.type;
			}
			
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
	FastNoiseSIMD::FreeNoiseSet(dirtNoise);
	FastNoiseSIMD::FreeNoiseSet(stoneNoise);
	FastNoiseSIMD::FreeNoiseSet(sandNoise);
	FastNoiseSIMD::FreeNoiseSet(mountainNoise);
	FastNoiseSIMD::FreeNoiseSet(plainsNoise);
	FastNoiseSIMD::FreeNoiseSet(caveNoise);

#pragma region Perlin Worm

	for (int i = 0; i < 20; i++)
	{
		float x = getRandomInt(rng, 10, w - 10);
		float y = getRandomInt(rng, 51, h - 10);

		int wormLength = getRandomInt(rng, getRandomInt(rng, 100, w / 4), getRandomInt(rng, 100, h / 4));
		
		float radius = 1.5f;

		if (getRandomChance(rng, 0.5f))
		{
			generateWorm(gameMap, x, y, wormLength);
		}
	}

#pragma endregion

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

void generateWorm(GameMap& gameMap, float x, float y, int length, float radius)
{

	std::ranlux24_base rng(gameMap.seed);

	float dirX = getRandomFloat(rng, -1, 1);
	float dirY = getRandomFloat(rng, -1, 1);

	// distance to change direction
	int changeDirTime = getRandomInt(rng, 5, 20);

	for (int j = 0; j < length; j++)
	{
		
		// Clear Circle around positon
		radius = getRandomFloat(rng, 0.5f, radius);

		int intRadius = std::ceil(radius);
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
		radius = std::clamp(radius, 2.2f, 8.5f);

	}

}
