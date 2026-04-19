#include "saveMap.h"
#include <asserts.h>
#include <filesystem>
#include <iostream>
#include <gameMap.h>
#include <nlohmann/json.hpp>

#include <entityIdHolder.h>
#include <player.h>
#include <entities/slime.h>
#include <entities/blueSlime.h>
#include <entities/zombie.h>

#include "entities/droppedItem.h"

const int VERSION = 1;

struct BlockSave1{
	std::uint16_t type = 0;
	std::uint16_t variation = 0;

	Block toBlock() const
	{
		Block b;
		b.type = type;
		b.variation = variation;
		return b;
	}
	Wall toWall() const
	{
		Wall b;
		b.type = type;
		b.variation = variation;
		return b;
	}

};

BlockSave1 ToBlockSave1(const Block& b)
{
	BlockSave1 bs;
	bs.type = b.type;
	bs.variation = b.variation;
	return bs;
}
BlockSave1 ToBlockSave1(const Wall& b)
{
	BlockSave1 bs;
	bs.type = b.type;
	bs.variation = b.variation;
	return bs;
}

bool saveBlockDataToFile(std::vector<Block> &blocks, std::vector<Wall> &walls, int w, int h, const char* fileName)
{
	std::cout << "Saving Block Data To File: " << fileName << "\n";
	std::ofstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return false; }

	// Development
	permaAssertDevelopement(blocks.size() >= w * h);
	permaAssertDevelopement(!blocks.empty());
	permaAssertDevelopement(walls.size() >= w * h);
	permaAssertDevelopement(!walls.empty());

	// Production
	if (blocks.size() != w * h) { return false; }
	if (blocks.empty()) { return false; }

	if (walls.size() != w * h) { return false; }
	if (walls.empty()) { return false; }

	f.write((char*)&VERSION, sizeof(VERSION));
	f.write((char*)&w, sizeof(w));
	f.write((char*)&h, sizeof(h));

	size_t blockCount = static_cast<size_t>(w) * h;
	
	// Write Block Data
	f.write((char*)blocks.data(), sizeof(Block) * blockCount);
	for (size_t i = 0; i < blocks.size(); i++)
	{
		auto b = ToBlockSave1(blocks[i]);
		f.write((char*)&b, sizeof(b));
	}

	// Write Wall Data
	f.write((char*)walls.data(), sizeof(Wall) * blockCount);
	for (size_t i = 0; i < walls.size(); i++)
	{
		auto b = ToBlockSave1(walls[i]);
		f.write((char*)&b, sizeof(b));
	}

	f.close();

	return true;
}

bool loadBlockDataFromFile(std::vector<Block>& blocks, std::vector<Wall> &walls, int& w, int& h, const char* fileName)
{
	blocks.clear();
	walls.clear();
	w = 0;
	h = 0;
	
	std::ifstream f(fileName, std::ios::binary);
	if (!f.is_open()) { return false; }

	int readVersion = 0;

	// Read Dimensions
	f.read((char*)&readVersion, sizeof(readVersion));
	f.read((char*)&w, sizeof(w));
	f.read((char*)&h, sizeof(h));

	if (!f || w <= 0 || h <= 0) return false;
	if (w > 10000 || h > 10000) return false;

	switch (readVersion)
	{
		case 1:
		{
			size_t blockCount = static_cast<size_t>(w) * h;
				
				// Load Block Data
			blocks.resize(blockCount);
			for (size_t i = 0; i < blockCount; i++)
			{
				BlockSave1 bs;
				f.read((char*)&bs, sizeof(bs));
				if (!f)
				{
					blocks.clear();
					w = 0;
					h = 0;
					f.close();
					return false;
				}
				blocks[i] = bs.toBlock();

			}
			// Load Wall Data
			walls.resize(blockCount);
			for (size_t i = 0; i < blockCount; i++)
			{
				BlockSave1 bs;
				f.read((char*)&bs, sizeof(bs));
				if (!f)
				{
					walls.clear();
					w = 0;
					h = 0;
					f.close();
					return false;
				}
				walls[i] = bs.toWall();

			}

			break;
		}
		default:
		{
			blocks.clear();
			walls.clear();
			w = 0;
			h = 0;
			f.close();
			return false;
		}
	}

	for (size_t i = 0; i < blocks.size(); i++)
	{
		blocks[i].sanatize();
		walls[i].sanatize();
	}

	f.close();
	return true;
}

bool writeEntireFile(const char* fileName, const void* data, size_t size)
{
	std::ofstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return false; }

	f.write((const char*)data, size);

	f.close();

	return true;
}



bool readEntireFile(const char* fileName, const void* data, size_t size)
{


	std::ifstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return  false; }

	f.seekg(0, std::ios_base::end);

	size_t readSize = f.tellg();
	f.seekg(0, std::ios_base::beg);


	f.read((char*)data, std::min(size, readSize));

	//auto* data_vec = static_cast<unsigned char*>(const_cast<void*>(data));

	//std::vector<unsigned char> vec(data_vec, data_vec + size);

	if (!f)
	{
		data = nullptr;
		return false;
	}
	

	return true;
}

using Json = nlohmann::json;

void saveWorld(GameMap& gameMap, EntityHolder& entities, Player& player)
{
	//std::error_code errorCode;
	//std::filesystem::create_directory(RESOURCES_PATH "../saves", errorCode);
	
	saveWorldOnly(gameMap);

	saveEntitiesOnly(entities, player);

	//saveBlockDataToFile(gameMap.mapData, gameMap.wallData, gameMap.w, gameMap.h, RESOURCES_PATH "../saves/map.bin.tmp");
	/*
	//id holder
	{
		std::ofstream f(RESOURCES_PATH "../saves/idHolder.txt.tmp");
		f << entities.entities.size() + 1;//.idHolder.idCounter;
		f.close();
	}

	//Player
	{
		Json j = player.formatToJson();

		std::ofstream f(RESOURCES_PATH "../saves/player.txt.tmp");
		f << j.dump(2);
		f.close();
	}

	// Entities
	{
		Json j;

		for (auto &e : entities.entities)
		{
			j[std::to_string(e.first)] = e.second->formatToJson();
		}

		std::ofstream f(RESOURCES_PATH "../saves/entities.txt.tmp");
		f << j.dump(2);
		f.close();
		
	}

	std::filesystem::rename(RESOURCES_PATH "../saves/map.bin.tmp", RESOURCES_PATH "../saves/map.bin", errorCode);
	std::filesystem::rename(RESOURCES_PATH "../saves/idHolder.txt.tmp", RESOURCES_PATH "../saves/idHolder.txt", errorCode);
	std::filesystem::rename(RESOURCES_PATH "../saves/player.txt.tmp", RESOURCES_PATH "../saves/player.txt", errorCode);
	std::filesystem::rename(RESOURCES_PATH "../saves/entities.txt.tmp", RESOURCES_PATH "../saves/entities.txt", errorCode);
	*/
	// Create A Backup for each file

}

bool loadWorld(GameMap& gameMap, EntityHolder& entities, Player& player)
{
	gameMap = {};
	entities.entities.clear();
	player = {};
	entities.idHolder = {};

	// If original file fails load, load from back up
	// if backup files fails load, return false

	if (!loadBlockDataFromFile(gameMap.mapData, gameMap.wallData, gameMap.w, gameMap.h, RESOURCES_PATH "../saves/map.bin"))
	{
		return false;
	}

	// Id Holder
	{
		std::ifstream f(RESOURCES_PATH "../saves/idHolder.txt");

		if (!f.is_open()) { return false; }
		f >> entities.idHolder.idCounter;
		f.close();
	}

	// Player
	{
		std::ifstream f(RESOURCES_PATH "../saves/player.txt");
		if (!f.is_open())
		{
			return false;
		}

		Json j;
		j = Json::parse(f, nullptr, /*allow_exceptions=*/false);

		if (!player.loadFromJson(j))
		{
			return false;
		}
	}

	// Entities
	{
		std::ifstream f(RESOURCES_PATH "../saves/entities.txt");
		if (!f.is_open())
		{
			return false;
		}

		Json j;
		j = Json::parse(f, nullptr, /*allow_exceptions=*/false);
		
		for (auto it = j.begin(); it != j.end(); ++it)
		{
			const std::string& keyStr = it.key();
			bool isNumberic = !keyStr.empty() && std::all_of(keyStr.begin(), keyStr.end(), ::isdigit);

			if (!isNumberic) continue;

			std::uint64_t id = 0;

			for (auto c : keyStr)
			{
				id *= 10;
				id += c - '0';
			}

			Json& entityJson = it.value();

			int entityType = 0;

			if (entityJson["entityType"].is_number()) // getEntityList()
			{
				entityType = entityJson["entityType"];

				switch (entityType)
				{
				case EntityType_Slime:
				{
					Slime slime;
					if (slime.loadFromJson(entityJson))
					{
						entities.entities[id] = std::make_unique<Slime>(slime);
					}
					break;
				}
				case EntityType_DroppedItem:
					{
					DroppedItem item;
						if (item.loadFromJson(entityJson))
						{
							entities.entities[id] = std::make_unique<DroppedItem>(item);
						}
					}
				}
			}

		}

	}
	return true;

}

void saveWorldOnly(GameMap& gameMap)
{
	std::error_code errorCode;
	std::filesystem::create_directory(RESOURCES_PATH "../saves", errorCode);

	saveBlockDataToFile(gameMap.mapData, gameMap.wallData, gameMap.w, gameMap.h, RESOURCES_PATH "../saves/map.bin.tmp");

	std::filesystem::rename(RESOURCES_PATH "../saves/map.bin.tmp", RESOURCES_PATH "../saves/map.bin", errorCode);

}

void saveEntitiesOnly(EntityHolder& entities, Player& player)
{
	std::error_code errorCode;
	std::filesystem::create_directory(RESOURCES_PATH "../saves", errorCode);

	// Id Holder
	{
		std::ofstream f(RESOURCES_PATH "../saves/idHolder.txt.tmp");
		f << entities.entities.size() + 1;
		f.close();
	}

	// Player
	{
		Json j = player.formatToJson();

		std::ofstream f(RESOURCES_PATH "../saves/player.txt.tmp");
		f << j.dump(2);
		f.close();
	}

	// Entities
	{
		Json j;

		for (auto& e : entities.entities)
		{
			j[std::to_string(e.first)] = e.second->formatToJson();
		}

		std::ofstream f(RESOURCES_PATH "../saves/entities.txt.tmp");
		f << j.dump(2);
		f.close();

	}


	std::filesystem::rename(RESOURCES_PATH "../saves/idHolder.txt.tmp", RESOURCES_PATH "../saves/idHolder.txt", errorCode);
	std::filesystem::rename(RESOURCES_PATH "../saves/player.txt.tmp", RESOURCES_PATH "../saves/player.txt", errorCode);
	std::filesystem::rename(RESOURCES_PATH "../saves/entities.txt.tmp", RESOURCES_PATH "../saves/entities.txt", errorCode);

}
