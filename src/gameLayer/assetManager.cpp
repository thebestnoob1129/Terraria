#include "assetManager.h"

void AssetManager::loadAll()
{
	dirt = LoadTexture(RESOURCES_PATH "Dirt\\Dirt_01-128x128.png");

	textures = LoadTexture(RESOURCES_PATH "textures.png");
	walls = LoadTexture(RESOURCES_PATH "texturesWithBackgroundVersion.png");
	frame = LoadTexture(RESOURCES_PATH "frame.png");
	tree = LoadTexture(RESOURCES_PATH "treetextures.png");
	/*
	brick = LoadTexture(RESOURCES_PATH "Brick\\Brick_01-128x128.png");
	metal = LoadTexture(RESOURCES_PATH "Metal\\Metal_01-128x128.png");
	plaster = LoadTexture(RESOURCES_PATH "Plaster\\Plaster_01-128x128.png");
	stone = LoadTexture(RESOURCES_PATH "Stone\\Stone_01-128x128.png");
	tile = LoadTexture(RESOURCES_PATH "Tile\\Tile_01-128x128.png");
	wood = LoadTexture(RESOURCES_PATH "Wood\\Wood_01-128x128.png");
	*/
}
