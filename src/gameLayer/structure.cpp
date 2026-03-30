#include "structure.h"

static std::ranlux24_base rng(std::random_device{}());

void Structure::create(int w, int h)
{
	*this = {};
	mapData.resize(w * h);
	wallData.resize(w * h);

	this->w = w;
	this->h = h;

	// Use To Set Random Variations Across tiles.
	for (auto& e : mapData) { e = {}; e.variation = getRandomInt(rng, 0, 3); } // Clears all Block Data
	for (auto& e : wallData) { e = {}; e.variation = getRandomInt(rng, 0, 3); } // Clears all Block Data
}

// Gets All Tile Data
Block& Structure::getBlockUnsave(int x, int y)
{
	permaAssertCommentDevelopement(mapData.size() == w * h, "Map Data Not Initialized");

	permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getBlocUnsafe out of bounds error");

	return mapData[x + y * w];

}

Block* Structure::getBlockSafe(int x, int y)
{
	permaAssertCommentDevelopement(mapData.size() == w * h, "Map Data Not Initialized");

	if (x < 0 || y < 0 || x >= w || y >= h) { return nullptr; }

	return &mapData[x + y * w];
}
Wall& Structure::getWallUnsave(int x, int y)
{
	permaAssertCommentDevelopement(wallData.size() == w * h, "Wall Data Not Initialized");

	permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getWallUnsafe out of bounds error");

	return wallData[x + y * w];

}

Wall* Structure::getWallSafe(int x, int y)
{
	permaAssertCommentDevelopement(wallData.size() == w * h, "Wall Data Not Initialized");

	if (x < 0 || y < 0 || x >= w || y >= h) { return nullptr; }

	return &wallData[x + y * w];
}

void Structure::copyFromMap(GameMap& map, Vector2 start, Vector2 end)
{
	if (end.x > map.w) { end.x = map.w - 1; }
	if (start.x > map.w) { end.x = map.w + 1; }

	if (end.y > map.h) { end.y = map.h - 1; }
	if (start.y > map.h) { end.y = map.h + 1; }

	if (end.x < 0) { end.x = 0; }
	if (end.y < 0) { end.y = 0; }

	if (start.x < 0) { start.x = 0; }
	if (start.y < 0) { start.y = 0; }

	if (start.x > end.x) { std::swap(start.x, end.x); }
	if (start.y > end.y) { std::swap(start.y, end.y); }

	Vector2 size = Vector2{ end.x - start.x + 1, end.y - start.y + 1 };

	if (size.x > map.w) { return; }
	if (size.y > map.h) { return; }

	create(size.x, size.y);

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			getBlockUnsave(x, y) = map.getBlockUnsave(x + start.x, y + start.y);
		}
	}

}

void Structure::pasteIntoMap(GameMap& map, Vector2 start)
{
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			auto b = map.getBlockSafe(x + start.x, y + start.y);

			if (b)
			{
				std::cout << b << "\n";
				*b = getBlockUnsave(x, y);
			}
		}
	}
	
}
