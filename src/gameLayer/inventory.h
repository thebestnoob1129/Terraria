#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

#include <raylib.h>
#include <vector>
#include <items.h>

struct AssetManager;



struct Inventory
{

	Item currentItemHeld;

	Rectangle rectangle;

	std::vector<Item> inventoryList;

	void render(AssetManager &assetManager);
	void addItem();
	void removeItem();
	

};

struct InventorySlot
{
	Item item;

	void render(AssetManager &assetManager, Inventory &inventory, Rectangle rect);
	void update();
	void addItem();
	void removeItem();

};

#endif
