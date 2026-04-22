#include "gamePlay.h"

#include <ui.h>
#include <physics.h>
#include <entity.h>
#include <entities/droppedItem.h>
#include <entities/slime.h>
#include <entities/blueSlime.h>
#include <entities/zombie.h>
#include <entities/eskimo_zombie.h>
#include <entities/mummy.h>

#include <unordered_map>
#include "audio.h"
#include <settings.h>

#include <helpers.h>
#include <imgui.h>
#include <assetManager.h>

DrawBackground background;
bool gameplayRunning;

Rectangle GamePlay::getInventoryRectangle(float w, float h)
{
	Rectangle inventoryRectangle = {};

	inventoryRectangle.height = GetScreenHeight() * 0.30;
	inventoryRectangle.width = inventoryRectangle.height * 3;

	inventoryRectangle = placeRectangleTopLeftCorner(inventoryRectangle, inventoryRectangle.width * 0.1f);

	inventoryRectangle.x += GetScreenWidth() * 0.01f;
	inventoryRectangle.y += GetScreenHeight() * 0.01f;
	return inventoryRectangle;
}

void GamePlay::spawnSlime(Vector2 position)
{
	std::unique_ptr<Mummy> newEntity = std::make_unique<Mummy>();

	// Teleport the new entity to spawn position and add to holder
	newEntity->physics.teleport(position);

	auto id = entities.idHolder.getEntityIdAndIncrement();
	entities.entities[id] = std::move(newEntity);

	saveEntitiesOnly(entities, player);
}

void GamePlay::spawnDroppedItem(Vector2 position, int type)
{
	DroppedItem droppedItem;

	std::ranlux24_base rng(std::random_device{}());

	Vector2 newPosition = Vector2{ position.x + getRandomFloat(rng), position.y };

	droppedItem.teleport(newPosition);
	droppedItem.itemType = type;

	auto id = entities.idHolder.getEntityIdAndIncrement();

	entities.entities[id] = std::make_unique<DroppedItem>(droppedItem);

}

void GamePlay::spawnRandomEnemy(Vector2 playerPosition)
{
	std::cout << "Spawn Random Entity \n";
	// Despawn Entity that is furthest Away From Player
	if (entities.entities.size() >= maxEnemies)
	{
		// Despawn Entities outside of range
		int furthestDistance = 0;
		Entity* ent = {};
		int id = 0;

		for (auto& e : entities.entities) // Scan For Entities
		{
			if (Vector2Distance(playerPosition, e.second->getPosition()) >= furthestDistance) // Compare Distance To Last Distance
			{
				std::cout << "Despawn Entity \n";
				ent = e.second.get();
				id = e.first;
				furthestDistance = Vector2Distance(playerPosition, e.second->getPosition());
			}
		}

		// Despawn Entity
		if (ent != nullptr)
		{
			ent->life = -1;
			// Get Entity Id
			entities.entities.erase(id);
		}
	}

	// Spawn Entity In New Range
	std::ranlux24_base rng(std::random_device{}());

	// Choose a random entity type from the EntityList (skip player at 0)
	int chosen = getRandomInt(rng, Entity_Slime, ENTITY_COUNT - 1);

	// Create the entity based on chosen type
	std::unique_ptr<Entity> newEntity;
	switch (chosen)
	{
	case Entity_Slime:
		newEntity = std::make_unique<Slime>();
		break;
	case Entity_BlueSlime:
		newEntity = std::make_unique<BlueSlime>();
		break;
	case Entity_Mummy:
		newEntity = std::make_unique<Mummy>();
		break;
	case Entity_Zombie:
		newEntity = std::make_unique<Zombie>();
		break;
	case Entity_Zombie_Eskimo:
		newEntity = std::make_unique<EskimoZombie>();
	default:
		break;
	}

	// Get New Spawn Location (random offset from player)
	Vector2 spawnPosition = Vector2{
		static_cast<float>(playerPosition.x + getRandomInt(rng, -spawnRange, spawnRange)),
		static_cast<float>(playerPosition.y + getRandomInt(rng, -spawnRange, spawnRange))
	};

	// Teleport the new entity to spawn position and add to holder
	newEntity->physics.teleport(spawnPosition);

	auto id = entities.idHolder.getEntityIdAndIncrement();
	entities.entities[id] = std::move(newEntity);

	saveEntitiesOnly(entities, player);

}

bool GamePlay::init(AssetManager &assetManager)
{
	generateWorld(gameMap, 1000, 500);

	// Camera
	camera.target = Vector2{ 20, 100 };
	camera.rotation = 0.0f;
	camera.zoom = 100.0f;
	// Player update

	player.physics.transform.position = Vector2{ 20, 100 };
	player.physics.texture = assetManager.player;
	player.physics.transform.w = 0.9f;
	player.physics.transform.h = 1.8f;

	spawnRandomEnemy(player.getPosition());

	return true;
}

bool GamePlay::update(AssetManager &assetManager)
{
	if (gameMap.shouldSave)
	{
		saveWorld(gameMap, entities, player);
		gameMap.shouldSave = false;
	}
	
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	// Game Parameters

	camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	ClearBackground({ 75, 75, 150, 255 });

	if (IsKeyPressed(KEY_F10)) { showImgui = !showImgui; }

#pragma region Camera Movement
	camera.target = player.physics.transform.position;

	// Clamp Camera
	{
		float zoom = camera.zoom;

		float screenWidth = GetScreenWidth();
		float screenHeight = GetScreenHeight();

		float halfViewWidth = (screenWidth * 0.5f) / zoom;
		float halfViewHeight = (screenHeight * 0.5f) / zoom;

		float minX = halfViewWidth;
		float maxX = gameMap.w - halfViewWidth;
		float minY = halfViewHeight;
		float maxY = gameMap.h - halfViewHeight;

		camera.target.x = Clamp(camera.target.x, minX, maxX);
		camera.target.y = Clamp(camera.target.y, minY, maxY);

		if (maxX < minX)
		{
			camera.target.x = gameMap.w * 0.5f;
		}
		else
		{
			camera.target.x = Clamp(camera.target.x, minX, maxX);
		}

		if (maxY < minY)
		{
			camera.target.y = gameMap.h * 0.5f;
		}
		else
		{
			camera.target.y = Clamp(camera.target.y, minY, maxX);
		}

	}

#pragma endregion

#pragma region Create / Destroy Blocks
	bool insideInventoryMenu = false;
	if (insideInventory &&
		CheckCollisionPointRec(GetMousePosition(), inventory.rectangle))
	{
		insideInventoryMenu = true;
	}

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);
	int blockX = static_cast<int>(floor(worldPos.x));
	int blockY = static_cast<int>(floor(worldPos.y));

	int createDistance = 5;

	auto selectedBlock = static_cast<Block>(creativeSelectedBlock);

	Clamp(creativeSelectedBlock, 0, Block::BLOCK_COUNT - 1);

	if (showImgui)
	{
		// Copy and Paste Structure
		if (IsKeyPressed(KEY_ONE)) { selectionStart = Vector2{ (float)blockX, (float)blockY }; }
		if (IsKeyPressed(KEY_TWO)) { selectionEnd = Vector2{ (float)blockX, (float)blockY }; }
		if (IsKeyPressed(KEY_THREE)) { copyStructure.pasteIntoMap(gameMap, Vector2{ (float)blockX, (float)blockY }); }

		if (selectionStart.x > selectionEnd.x) { std::swap(selectionStart.x, selectionEnd.x); }
		if (selectionStart.y > selectionEnd.y) { std::swap(selectionStart.y, selectionEnd.y); }

	}

	if (!showImgui)
	{
		/// Place Blocks
		if (!insideInventoryMenu)
			if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				creativeSelectedBlock = gameMap.getBlockSafe(blockX, blockY)->type;
				//saveWorldOnly(gameMap);
			}

		if (!insideInventoryMenu)
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				if (blockX <= player.physics.transform.position.x + createDistance && blockX >= player.physics.transform.position.x - createDistance &&
					blockY <= player.physics.transform.position.y + createDistance && blockY >= player.physics.transform.position.y - createDistance)
				{
					/* Creative Mode Replace Blocks
					if (b) *b = {};
					*/
					auto b = gameMap.getBlockSafe(blockX, blockY);
					if (b) {
						if (b->type) {
							spawnDroppedItem({ static_cast<float>(blockX + 0.5f), static_cast<float>(blockY + 0.5f) }, b->type);
						}

						*b = {};
						//saveWorldOnly(gameMap);
					}
				}
			}

		/// Add Blocks
		if (!insideInventoryMenu)
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			{
				if (blockX <= player.physics.transform.position.x + createDistance && blockX >= player.physics.transform.position.x - createDistance &&
					blockY <= player.physics.transform.position.y + createDistance && blockY >= player.physics.transform.position.y - createDistance)
				{
					auto b = gameMap.getBlockSafe(blockX, blockY);
					if (b) b->type = creativeSelectedBlock;
				}
			}

		/// Open Inventory
		if (IsKeyPressed(KEY_TAB))
		{
			insideInventory = !insideInventory;
			std::cout << insideInventory << "\n";
		}

	}


#pragma endregion


#pragma region Entities

	auto updateEntityPhysics = [&](auto& entity, bool applyGravity = true)
		{
			if (applyGravity) { entity.physics.applyGravity(); }

			entity.physics.updateForces(deltaTime);
			entity.physics.resolveConstrains(gameMap);
			entity.physics.updateFinal();
		};

	// Player Controller 
	bool moving = false;
	bool falling = false;

	// Creative Move Movement
	static bool creative = false;
	if (creative) {
		if (IsKeyDown(KEY_W))
		{
			player.physics.transform.position.y -= player.physics.moveSpeed * deltaTime;
			moving = true;
		}
		if (IsKeyDown(KEY_S))
		{
			player.physics.transform.position.y += player.physics.moveSpeed * deltaTime;
			moving = true;
		}
	}
	// Natural Movement
	if (IsKeyDown(KEY_A))
	{
		player.physics.transform.position.x -= player.physics.moveSpeed * deltaTime;
		moving = true;
		player.animation.movingLeft = true;
	}
	if (IsKeyDown(KEY_D))
	{
		player.physics.transform.position.x += player.physics.moveSpeed * deltaTime;
		moving = true;
		player.animation.movingLeft = false;
	}
	if (IsKeyDown(KEY_SPACE)) player.physics.jump(15.0);

	// Falling Flag
	if (player.physics.downTouch)
	{
		falling = false;
	}
	else
	{
		falling = true;
	}

	// Falling Animation
	if (falling)
	{
		player.animation.setAnimation(2);
	}
	else if (moving)
	{
		player.animation.setAnimation(1);
	}
	else
	{
		player.animation.setAnimation(0);
	}
	player.animation.update(deltaTime, 0.08f, 7);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		int attackRange = 2;

		for (auto& e : entities.entities)
		{
			// If within attack range
			if (blockX <= e.second->physics.transform.position.x + attackRange && blockX >= e.second->physics.transform.position.x - attackRange &&
				blockY <= e.second->physics.transform.position.y + attackRange && blockY >= e.second->physics.transform.position.y - attackRange)
			{
				if (e.second->getEntityType() == EntityType_DroppedItem) { break; }
				e.second->OnHit(3);
			}
		}
	}

	updateEntityPhysics(player, !creative);

	// Entity Controller

	std::ranlux24_base rng(std::random_device{}());

	// Spawn Random Entites

	if (spawnTimer > 0.0f)
	{
		spawnTimer -= deltaTime;
		if (spawnTimer <= 0.0f)
		{
			spawnRandomEnemy(player.getPosition());
			spawnTimer = 10;
		}
	}


	for (auto it = entities.entities.begin(); it != entities.entities.end();) {

		// Update Entity Data
		EntityUpdateData updateData{
			player.getPosition(),
			rng,
			entities,
			it->first,
		};

		bool shouldKill = false;

		// Remove Entity From Container
		if (!it->second->update(deltaTime, updateData) || it->second->life <= 0) {
			shouldKill = true;
		}

		if (shouldKill) {
			if (it->second->getEntityType() != EntityType_DroppedItem) { spawnDroppedItem(it->second->getPosition(), it->second->getDrop()); }
			it = entities.entities.erase(it);
		}
		else {
			// Update Entity Physics
			updateEntityPhysics(*it->second, useGravity);
			it++;
		}

	}


#pragma endregion

#pragma region Time Cycle

	float cycle = getCycleTime(60 * 5);
	float saveCycle = getCycleTime(60 * 2);
	float dayFactor = getDayFactor(cycle);

	// Get each background needed for day / night

	int backgroundType = DrawBackground::forest;
	int musicType = Audio::musicForest;


	if (player.getPosition().x > gameMap.desertStart &&
		player.getPosition().x < gameMap.desertEnd)
	{
		backgroundType = DrawBackground::desert;
		musicType = Audio::musicDesert;
	}
	else if (player.getPosition().y > 150)
	{
		// Draw Cave Background
		backgroundType = DrawBackground::cave;
		musicType = Audio::musicCave;
		//background.draw(assetManager.caveBG, deltaTime, camera, gameMap.getMapSize());
	}
	background.setBackground(backgroundType);
	Audio::playMusic(musicType);
	background.draw(deltaTime, assetManager, camera, gameMap.getMapSize());

	if (saveCycle == 1)
	{
		gameMap.shouldSave = true;
	}
	/*
	else // Normal Day Cycle
	{

		// Night
		background.draw(assetManager.nightSky, deltaTime, camera, gameMap.getMapSize(), 0.3f, dayFactor);
		background.draw(assetManager.stars, deltaTime, camera, gameMap.getMapSize(), 0.3f, dayFactor);
		background.draw(assetManager.moon, deltaTime, camera, gameMap.getMapSize(), 0.3f, dayFactor);
		background.draw(assetManager.cloudsNight, deltaTime, camera, gameMap.getMapSize(), 0.3f, dayFactor);

		// Day
		background.draw(assetManager.sky, deltaTime, camera, gameMap.getMapSize(), 0.3f, 1 - dayFactor);
		background.draw(assetManager.sun, deltaTime, camera, gameMap.getMapSize(), 0.3f, 1 - dayFactor);
		background.draw(assetManager.clouds, deltaTime, camera, gameMap.getMapSize(), 0.3f, 1 - dayFactor);
	}
	*/

	// Change background based on day factor time with alpha

	// Load area background based on current world layer location with transition smoothness

	// changed the background color
	Color night = { 10, 10, 40, 255 };
	Color day = { 135, 206, 235, 255 };



#pragma endregion
	BeginMode2D(camera);

#pragma region Occulating Viewport 2D

	Vector2 topLeftView = GetScreenToWorld2D({ 0, 0 }, camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) }, camera);

	int startXView = static_cast<int>(floorf(topLeftView.x - 1));
	int endXView = static_cast<int>(ceilf(bottomRightView.x + 1));
	int startYView = static_cast<int>(floorf(topLeftView.y - 1));
	int endYView = static_cast<int>(ceilf(bottomRightView.y + 1));

	startXView = Clamp(startXView, 0, gameMap.w - 1);
	endXView = Clamp(endXView, 0, gameMap.w - 1);

	startYView = Clamp(startYView, 0, gameMap.h - 1);
	endYView = Clamp(endYView, 0, gameMap.h - 1);

#pragma endregion


	// Generate World Blocks
	for (int y = startYView; y < endYView; y++)
	{
		for (int x = startXView; x < endXView; x++)
		{
			auto& b = gameMap.getBlockUnsave(x, y);
			auto& w = gameMap.getWallUnsave(x, y);

			if (w.type != Block::air)
			{
				DrawTexturePro(
					assetManager.walls, getTextureAtlas(w.type, w.variation),
					{ static_cast<float>(x), static_cast<float>(y), 1, 1 },
					{ 0, 0 },// Origin From Top Left Corner
					0.0f, // Rotation
					GRAY);// Tint
			}

			if (b.type != Block::air)
			{
				DrawTexturePro(
					assetManager.textures, getTextureAtlas(b.type, b.variation),
					{ static_cast<float>(x), static_cast<float>(y), 1, 1 },
					{ 0, 0 },// Origin From Top Left Corner
					0.0f, // Rotation
					WHITE);// Tint
			}
		}
	}

#pragma region Entities

	for (auto& e : entities.entities) {
		e.second->render(assetManager);
	}
	// Player
	Transform2D playerSprite = player.physics.transform;
	playerSprite.w = 1;
	playerSprite.h = 2;

	playerSprite.position.y -= (playerSprite.h - player.physics.transform.h) / 2;

	player.render(assetManager);

#pragma endregion

#pragma region Creative


	// Cursor
	DrawTexturePro(
		assetManager.textures,
		getTextureAtlas(selectedBlock.type, 0),
		{ static_cast<float>(blockX), static_cast<float>(blockY), 1, 1 },
		{ 0, 0 },// Origin From Top Left Corner
		0.0f, // Rotation
		WHITE);// Tint
	//
	if (!insideInventoryMenu) {
		DrawTexturePro(
			assetManager.frame,
			getTextureAtlas(0, 0),
			{ static_cast<float>(blockX), static_cast<float>(blockY), 1, 1 },
			{ 0, 0 },// Origin From Top Left Corner
			0.0f, // Rotation
			WHITE);// Tint
	}

	if (showImgui)
	{

		// Block Selection
		Rectangle rect = {};
		rect.x = selectionStart.x;
		rect.y = selectionStart.y;
		rect.width = selectionEnd.x - selectionStart.x;
		rect.height = selectionEnd.y - selectionStart.y;

		rect.width++;
		rect.height++;

		DrawRectangleLinesEx(rect, 0.1f, { 53, 86, 223, 255 });

		// Player Outline
		DrawRectangleLinesEx(player.physics.transform.getAABB(), 0.1f, Color{ 52, 164, 39, 255 });

	}

#pragma endregion 

	EndMode2D();
#pragma region ui

	float w = GetScreenWidth();
	float h = GetScreenHeight();

	Rectangle hearthRectangle;

	hearthRectangle.height = h * 0.05f;
	hearthRectangle.width = hearthRectangle.height * player.getMaxLife() / 2;

	hearthRectangle = placeRectangleTopRightCorner(hearthRectangle, w);
	//DrawRectangle(hearthRectangle.x, hearthRectangle.y, hearthRectangle.width, hearthRectangle.height, RED);


	// Draw Heart Area
	auto drawHeart = [&](int amount, Color col = WHITE)
		{
			int hearts = (amount + 1) / 2; // total life / 2

			for (int i = 0; i < hearts; i++)
			{
				Rectangle oneHeart = hearthRectangle;
				oneHeart.width = oneHeart.height;
				oneHeart.x += oneHeart.width * i;
				// if hearts are > 10 add another row from beginning

				int remaining = amount - (i * 2); // HP covered by this heart
				int tex = 0; // sprite index: 0 = empty, 1 = half, 2 = full (adjust if your sheet differs)

				if (remaining >= 2) tex = 3;   // full heart
				else if (remaining == 1) tex = 4; // half heart
				else tex = 0; // empty (shouldn't happen normally)

				int flash = 0;

				DrawTexturePro(
					assetManager.hearts,
					getTextureAtlas(tex - flash, 0, assetManager.hearts.width / 5, assetManager.hearts.height),
					oneHeart,
					{ 0, 0 },
					0.0f,
					col
				);

			}
		};

	drawHeart(player.getMaxLife(), GRAY);
	drawHeart(player.life);

	/// Inventory

	if (insideInventory)
	{
		Rectangle invRect = {
			0,
			0,
			static_cast<float>(GetScreenWidth()),
			static_cast<float>(GetScreenHeight())
		};
		inventory.render(assetManager);
	}


#pragma endregion

#pragma region ImGui
	// Normal ImGui Implementation

	if (showImgui) {

		ImGui::Begin("Game Controller");

		static int genWorldWidth = 2000;
		static int genWorldHeight = 1000;
		static int genSeed = 12345;

		// Camera Data
		ImGui::BeginChild("Camera Data");//, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.2f));
		ImGui::SliderFloat("Camera zoom:", &camera.zoom, 10, 150);
		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Player Data");// , ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.3f));

		ImGui::Checkbox("Creative", &creative);

		Vector2 teleportPosition = { 20, 100 };

		ImGui::InputFloat2("Player Position:", &player.physics.transform.position.x);
		ImGui::InputFloat2("Player Velocity:", &player.physics.velocity.x);
		ImGui::InputFloat2("Player Acceleration:", &player.physics.acceleration.x);
		ImGui::SliderFloat("Player Speed: ", &player.physics.moveSpeed, 1, 100);
		ImGui::InputFloat2("Teleport To:", &teleportPosition.x);

		if (ImGui::Button("Teleport Player"))
		{
			player.teleport(teleportPosition);
		}
		if (ImGui::Button("Hurt Player"))
		{
			player.OnHit(5);
		}
		if (ImGui::Button("Heal Player"))
		{
			player.life += 1;
		}
		ImGui::Text(std::to_string(player.life).c_str());
		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Inventory Data");

		if (insideInventory)
		{
			std::string x = std::to_string(inventory.rectangle.x);
			std::string y = std::to_string(inventory.rectangle.y);
			std::string w = std::to_string(inventory.rectangle.width);
			std::string h = std::to_string(inventory.rectangle.height);


			ImGui::Text("Inventory:");
			ImGui::Text(x.c_str());
			ImGui::Text(y.c_str());
			ImGui::Text(w.c_str());
			ImGui::Text(h.c_str());
		}

		ImGui::EndChild();

		ImGui::Separator();

		// World Data
		ImGui::BeginChild("World Data");// , ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.4f));

		ImGui::InputInt("Seed:", &genSeed, 1, 100);
		ImGui::InputInt("World Width:", &genWorldWidth, 1, 100);
		ImGui::InputInt("World Height:", &genWorldHeight, 1, 100);

		if (ImGui::Button("Create New World"))
		{
			gameMap.seed = genSeed;
			generateWorld(gameMap, genWorldWidth, genWorldHeight);
		}
		if (ImGui::Button("Save World"))
		{
			saveWorld(gameMap, entities, player);
		}
		if (ImGui::Button("Load World"))
		{
			if (!loadWorld(gameMap, entities, player))
			{
				return false;
			}
		}
		static int texturePackID = -1;
		ImGui::InputInt("Texture Pack ID:", &texturePackID, 1, 2);

		if (ImGui::Button("Load Texture Pack")) { assetManager.LoadTexturePack(texturePackID); }

		if (ImGui::Button("Copy"))
		{
			copyStructure.copyFromMap(gameMap, selectionStart, selectionEnd);
		}
		ImGui::Text("Entities:");
		ImGui::Text(std::to_string(entities.entities.size()).c_str());
		if (ImGui::Button("Spawn Entity"))
		{
			spawnRandomEnemy(player.getPosition());
		}

		if (ImGui::Button("Spawn Mummy"))
		{
			spawnSlime(player.getPosition());
		}

		bool showEntityHitBox = false;
		if (ImGui::Button("Show Hit Box")) { showEntityHitBox = !showEntityHitBox; }
		if (showEntityHitBox)
		{
			for (auto &e : entities.entities)
			{
				auto aabb = e.second->physics.transform.getAABB();

				DrawRectangleLines(aabb.x, aabb.y, aabb.width, aabb.height, RED);
			}
		}

		if (ImGui::Button("Hurt All Entities"))
		{
			for (auto& e : entities.entities)
			{
				e.second->OnHit(1, RED);
			}
		}


		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Save Data");// , ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.5f));

		ImGui::InputText("Save Name", saveName, sizeof(saveName));

		if (ImGui::Button("Save To File"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += saveName;
			path += ".bin";

			saveBlockDataToFile(copyStructure.mapData,
				copyStructure.wallData,
				copyStructure.w, copyStructure.h,
				path.c_str());
		}
		if (ImGui::Button("Load To File"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += saveName;
			path += ".bin";

			loadBlockDataFromFile(copyStructure.mapData,
				copyStructure.wallData,
				copyStructure.w, copyStructure.h,
				path.c_str());
		}

		if (ImGui::Button("Save Settings"))
		{
			saveSettings();
		}

		if (ImGui::Button("Load Settings"))
		{
			loadSettings();
		}

		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Creative ");// , ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.6f));
		if (creative) {
			for (int i = 0; i < Block::BLOCK_COUNT; i++)
			{
				auto atlas = getTextureAtlas(i, 0);
				atlas.x /= assetManager.textures.width;
				atlas.width /= assetManager.textures.width;
				atlas.y /= assetManager.textures.height;
				atlas.height /= assetManager.textures.height;

				ImGui::PushID(i);

				std::string str = std::to_string(i);
				const char* char_ptr = str.c_str();

				ImTextureID tex = static_cast<ImTextureID>(static_cast<intptr_t>(assetManager.textures.id));
				if (ImGui::ImageButton(char_ptr, tex,
					{ 35, 35 }, { atlas.x, atlas.y },
					{ atlas.x + atlas.width, atlas.y + atlas.height }))
				{
					creativeSelectedBlock = i;
				}

				ImGui::PopID();

				if (i % 10 != 0)
				{
					ImGui::SameLine();
				}

			}
		}
		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Audio ");// , ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.6f));

		ImGui::SliderFloat("Master Volume", &getSettings().masterVolume, 0, 1);
		ImGui::SliderFloat("Sound Volume", &getSettings().soundsVolume, 0, 1);
		ImGui::SliderFloat("Music Volume", &getSettings().musicVolume, 0, 1);

		if (ImGui::Button("Play sound"))
		{
			Audio::playSound(Audio::placeBlock);
		}

		if (ImGui::Button("Play Forest Music"))
		{
			Audio::playMusic(Audio::musicForest);
		}

		if (ImGui::Button("Play Desert Music"))
		{
			Audio::playMusic(Audio::musicDesert);
		}

		ImGui::EndChild();

		ImGui::End();
	}


#pragma endregion
	DrawFPS(10, 10);

	return true;
}
