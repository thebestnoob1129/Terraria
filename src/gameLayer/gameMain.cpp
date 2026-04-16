#include "gameMain.h"

// Add Trees

struct GameData
{
	GameMap gameMap = {};

	// Camera Information
	Camera2D camera = {};
	float cameraSpeed = 10;

	// World Information
	DrawBackground background;
	bool useGravity = true;
	float currentTime;

	// Creative Information
	int creativeSelectedBlock = Block::dirt;
	Structure copyStructure = {};
	Vector2 selectionStart = {};
	Vector2 selectionEnd = {};

	// Save Information
	char saveName[100]{};

	// Entity Information
	Player player = {};
	EntityHolder entities;
	int maxEnemies = 50;
	int spawnRange = 50; // Blocks
	float spawnTimer = 10;

}gameData;

AssetManager assetManager;
DeveloperWindow devWindow;

bool showImgui = false;

// Functions ***************
#pragma region Functions
void spawnDroppedItem(Vector2 position, int type) {
	DroppedItem droppedItem;

	std::ranlux24_base rng(std::random_device{}());

	Vector2 newPosition = Vector2{ position.x + getRandomFloat(rng), position.y };

	droppedItem.teleport(newPosition);
	droppedItem.itemType = type;

	auto id = gameData.entities.idHolder.getEntityIdAndIncrement();

	gameData.entities.entities[id] = std::make_unique<DroppedItem>(droppedItem);
}

void spawnRandomEnemy(Vector2 playerPosition)
{
	std::cout << "Spawn Random Entity \n";
	// Despawn Entity that is furthest Away From Player
	if (gameData.entities.entities.size() >= gameData.maxEnemies)
	{
		// Despawn Entities outside of range
		int furthestDistance = 0;
		Entity* ent = {};
		int id = 0;

		for (auto& e : gameData.entities.entities) // Scan For Entities
		{
			if ( Vector2Distance(playerPosition, e.second->getPosition()) >= furthestDistance) // Compare Distance To Last Distance
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
			gameData.entities.entities.erase(id);
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
		static_cast<float>(playerPosition.x + getRandomInt(rng, -gameData.spawnRange, gameData.spawnRange)),
		static_cast<float>(playerPosition.y + getRandomInt(rng, -gameData.spawnRange, gameData.spawnRange))
	};

	// Teleport the new entity to spawn position and add to holder
	newEntity->physics.teleport(spawnPosition);

	auto id = gameData.entities.idHolder.getEntityIdAndIncrement();
	gameData.entities.entities[id] = std::move(newEntity);

}

#pragma endregion
// ***************** Game Loop 

bool init_game()
{
	Audio::init();
	loadSettings();

	assetManager.loadAll();
	//assetManager.LoadTexturePack(0);

	generateWorld(gameData.gameMap,1000, 500);

	// Camera
	gameData.camera.target = Vector2{ 20, 100 };
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 100.0f;
	// Player update

	gameData.player.physics.transform.position = Vector2{ 20, 100 };
	gameData.player.physics.texture = assetManager.player;
	gameData.player.physics.transform.w = 0.9f;
	gameData.player.physics.transform.h = 1.8f;

	spawnRandomEnemy(gameData.player.getPosition());

	return true;
}

bool update_game()
{
	Audio::update();
	updateSettings(); // Automatically Change Settings if world Changed | Or Use On Apply Settings;
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	// Game Parameters

	gameData.camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	ClearBackground({ 75, 75, 150, 255 });

	if (IsKeyPressed(KEY_F10)) { showImgui = !showImgui; }

#pragma region Camera Movement
	gameData.camera.target = gameData.player.physics.transform.position;

	// Clamp Camera
	{
		float zoom = gameData.camera.zoom;

		float screenWidth = GetScreenWidth();
		float screenHeight = GetScreenHeight();

		float halfViewWidth = (screenWidth * 0.5f) / zoom;
		float halfViewHeight = (screenHeight * 0.5f) / zoom;

		float minX = halfViewWidth;
		float maxX = gameData.gameMap.w - halfViewWidth;
		float minY = halfViewHeight;
		float maxY = gameData.gameMap.h - halfViewHeight;

		gameData.camera.target.x = Clamp(gameData.camera.target.x, minX, maxX);
		gameData.camera.target.y = Clamp(gameData.camera.target.y, minY, maxY);

		if (maxX < minX)
		{
			gameData.camera.target.x = gameData.gameMap.w * 0.5f;
		}
		else
		{
			gameData.camera.target.x = Clamp(gameData.camera.target.x, minX, maxX);
		}

		if (maxY < minY)
		{
			gameData.camera.target.y = gameData.gameMap.h * 0.5f;
		}
		else
		{
			gameData.camera.target.y = Clamp(gameData.camera.target.y, minY, maxX);
		}

	}

#pragma endregion

#pragma region Create / Destroy Blocks
	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = static_cast<int>(floor(worldPos.x));
	int blockY = static_cast<int>(floor(worldPos.y));

	int createDistance = 5;

	auto selectedBlock = static_cast<Block>(gameData.creativeSelectedBlock);

	Clamp(gameData.creativeSelectedBlock, 0, Block::BLOCK_COUNT - 1);

	if (showImgui)
	{
		// Copy and Paste Structure
		if (IsKeyPressed(KEY_ONE)) { gameData.selectionStart = Vector2{ (float)blockX, (float)blockY }; }
		if (IsKeyPressed(KEY_TWO)) { gameData.selectionEnd = Vector2{ (float)blockX, (float)blockY }; }
		if (IsKeyPressed(KEY_THREE)) { gameData.copyStructure.pasteIntoMap(gameData.gameMap, Vector2{(float)blockX, (float)blockY});}

		if (gameData.selectionStart.x > gameData.selectionEnd.x) { std::swap(gameData.selectionStart.x, gameData.selectionEnd.x); }
		if (gameData.selectionStart.y > gameData.selectionEnd.y) { std::swap(gameData.selectionStart.y, gameData.selectionEnd.y); }

	}

	if (!showImgui)
	{
		if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			gameData.creativeSelectedBlock = gameData.gameMap.getBlockSafe(blockX, blockY)->type;
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			if (blockX <= gameData.player.physics.transform.position.x + createDistance && blockX >= gameData.player.physics.transform.position.x - createDistance &&
				blockY <= gameData.player.physics.transform.position.y + createDistance && blockY >= gameData.player.physics.transform.position.y - createDistance)
			{
				/* Creative Mode Replace Blocks
				if (b) *b = {};
				*/
				auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
				if (b) {
					if (b->type) {
						spawnDroppedItem({ static_cast<float>(blockX + 0.5f), static_cast<float>(blockY + 0.5f) }, b->type);
					}

					*b = {};
				}
			}
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			if (blockX <= gameData.player.physics.transform.position.x + createDistance && blockX >= gameData.player.physics.transform.position.x - createDistance &&
				blockY <= gameData.player.physics.transform.position.y + createDistance && blockY >= gameData.player.physics.transform.position.y - createDistance)
			{
				auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
				if (b) b->type = gameData.creativeSelectedBlock;
			}
		}

	}


#pragma endregion


#pragma region Entities
	float playerSpeed = 7;

	auto updateEntityPhysics = [&](auto& entity, bool applyGravity = true)
	{
		if (applyGravity) { entity.physics.applyGravity(); }

		entity.physics.updateForces(deltaTime);
		entity.physics.resolveConstrains(gameData.gameMap);
		entity.physics.updateFinal();
	};

	// Player Controller 

	if (IsKeyDown(KEY_W)) gameData.player.physics.transform.position.y -= playerSpeed * deltaTime;
	if (IsKeyDown(KEY_S)) gameData.player.physics.transform.position.y += playerSpeed * deltaTime;
	if (IsKeyDown(KEY_A)) gameData.player.physics.transform.position.x -= playerSpeed * deltaTime;
	if (IsKeyDown(KEY_D)) gameData.player.physics.transform.position.x += playerSpeed * deltaTime;
	if (IsKeyDown(KEY_SPACE)) gameData.player.physics.jump(10);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		int attackRange = 2;

		for (auto& e : gameData.entities.entities)
		{
			// If within attack range
			if (blockX <= e.second->physics.transform.position.x + attackRange && blockX >= e.second->physics.transform.position.x - attackRange &&
				blockY <= e.second->physics.transform.position.y + attackRange && blockY >= e.second->physics.transform.position.y - attackRange)
			{
				if (e.second->getEntityType() == EntityType_DroppedItem) { break; }
				std::cout << "Attempt Attack: " << 3 << "\n";
				e.second->OnHit(3);
			}
		}
	}

	updateEntityPhysics(gameData.player, gameData.useGravity);

	// Entity Controller

	std::ranlux24_base rng(std::random_device{}());

	// Spawn Random Entites

	if (gameData.spawnTimer > 0.0f)
	{
		gameData.spawnTimer -= deltaTime;
		if (gameData.spawnTimer <= 0.0f)
		{
			spawnRandomEnemy(gameData.player.getPosition());
			gameData.spawnTimer = 10;
		}
	}


	for (auto it = gameData.entities.entities.begin(); it != gameData.entities.entities.end();) {

		// Update Entity Data
		EntityUpdateData updateData{
			gameData.player.getPosition(),
			rng,
			gameData.entities,
			it->first,
		};

		bool shouldKill = false;

		// Remove Entity From Container
		if (!it->second->update(deltaTime, updateData) || it->second->life <= 0) {
			shouldKill = true;
		}

		if (shouldKill) {
			if (it->second->getEntityType() != EntityType_DroppedItem) { spawnDroppedItem(it->second->getPosition(), it->second->getDrop()); }
			it = gameData.entities.entities.erase(it);
		}
		else {
			// Update Entity Physics
			updateEntityPhysics(*it->second, gameData.useGravity);
			it++;
		}

	}	


#pragma endregion

#pragma region Time Cycle

	float cycle = getCycleTime(60 * 5);
	float dayFactor = getDayFactor(cycle);

	// Get each background needed for day / night
	
	
	if (gameData.player.getPosition().y > 200)
	{
		// Draw Cave Background
		gameData.background.draw(assetManager.caveBG, deltaTime, gameData.camera, gameData.gameMap.getMapSize());
	}
	else // Normal Day Cycle
	{

		// Night
		gameData.background.draw(assetManager.nightSky, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, dayFactor);
		gameData.background.draw(assetManager.stars, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, dayFactor);
		gameData.background.draw(assetManager.moon, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, dayFactor);
		gameData.background.draw(assetManager.cloudsNight, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, dayFactor);

		// Day
		gameData.background.draw(assetManager.sky, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, 1 - dayFactor);
		gameData.background.draw(assetManager.sun, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, 1 - dayFactor);
		gameData.background.draw(assetManager.clouds, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, 1 - dayFactor);
		/*
		// Draw Sky ( immediate Switch
		if (dayFactor <= 0.5f) // night
		{
			gameData.background.draw(assetManager.nightSky, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, scaled);
			gameData.background.draw(assetManager.stars, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, scaled);
			gameData.background.draw(assetManager.moon, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, scaled);
			gameData.background.draw(assetManager.cloudsNight, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, scaled);
		}
		else  // night
		{
			gameData.background.draw(assetManager.sky, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, 1-scaled);
			gameData.background.draw(assetManager.sun, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, 1-scaled);
			gameData.background.draw(assetManager.clouds, deltaTime, gameData.camera, gameData.gameMap.getMapSize(), 0.3f, 1-scaled);
		}
		*/


	}
	
	// Change background based on day factor time with alpha

	// Load area background based on current world layer location with transition smoothness

	// changed the background color
	Color night = { 10, 10, 40, 255 };
	Color day = { 135, 206, 235, 255 };



#pragma endregion
	BeginMode2D(gameData.camera);

#pragma region Occulating Viewport 2D

	Vector2 topLeftView = GetScreenToWorld2D({ 0, 0 }, gameData.camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) }, gameData.camera);

	int startXView = static_cast<int>(floorf(topLeftView.x - 1));
	int endXView = static_cast<int>(ceilf(bottomRightView.x + 1));
	int startYView = static_cast<int>(floorf(topLeftView.y - 1));
	int endYView = static_cast<int>(ceilf(bottomRightView.y + 1));

	startXView = Clamp(startXView, 0, gameData.gameMap.w - 1);
	endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);

	startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
	endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);

#pragma endregion


	// Generate World Blocks
	for (int y = startYView; y < endYView; y++)
	{
		for (int x = startXView; x < endXView; x++)
		{
			auto& b = gameData.gameMap.getBlockUnsave(x, y);
			auto& w = gameData.gameMap.getWallUnsave(x, y);


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

	for (int i = 1; i < gameData.gameMap.h; i *= 50)
	{
		for (int x = 0; x < gameData.gameMap.w; x++)
		{
			auto b = gameData.gameMap.getBlockSafe(x, i);
			if (b) { b->type = Block::goldBlock; }
		}
	}

#pragma region Entities

	for (auto& e : gameData.entities.entities) {
		e.second->render(assetManager);
	}
	// Player
	Transform2D playerSprite = gameData.player.physics.transform;
	playerSprite.w = 1;
	playerSprite.h = 2;

	playerSprite.position.y -= (playerSprite.h - gameData.player.physics.transform.h) / 2;

	DrawTexturePro(
		assetManager.player,
		getTextureAtlas(0, 0, 32, 64),
		getRectangleForEntity(gameData.player.physics.transform, 1, 2),
		{ 0, 0 },// Origin From Top Left Corner
		0.0f,
		WHITE
	);

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
	DrawTexturePro(
		assetManager.frame,
		getTextureAtlas(0, 0),
		{ static_cast<float>(blockX), static_cast<float>(blockY), 1, 1 },
		{ 0, 0 },// Origin From Top Left Corner
		0.0f, // Rotation
		WHITE);// Tint

	if (showImgui)
	{

		// Block Selection
		Rectangle rect = {};
		rect.x = gameData.selectionStart.x;
		rect.y = gameData.selectionStart.y;
		rect.width = gameData.selectionEnd.x - gameData.selectionStart.x;
		rect.height = gameData.selectionEnd.y - gameData.selectionStart.y;

		rect.width++;
		rect.height++;

		DrawRectangleLinesEx(rect, 0.1f, { 53, 86, 223, 255 });

		// Player Outline
		DrawRectangleLinesEx(gameData.player.physics.transform.getAABB(), 0.1f, Color{ 52, 164, 39, 255 });

	}

#pragma endregion 

	EndMode2D();
#pragma region Imgui
	/* Developer Window Implementaion
	if (showImgui)
	{
		DeveloperWindow devWindow;

		devWindow.render(gameData);
		
		if (ImGui::Button("#Camera Window"))
		{
			devWindow.camWindow = !devWindow.camWindow;
		}
		if (ImGui::Button("#Player Window"))
		{
			devWindow.playerWindow = !devWindow.playerWindow;
		}

		for (auto &w : devWindow.windows)
		{	
			if (w.enabled)
			{
				w.render(gameData);
			}
		}

	}
	*/

	// Normal Imgui Implementation
	
	if (showImgui) {

		ImGui::Begin("Game Controller");

		static int genWorldWidth = 900;
		static int genWorldHeight = 500;
		static int genSeed = 0;

		// Camera Data
		ImGui::BeginChild("Camera Data");//, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.2f));

		ImGui::SliderFloat("Camera zoom:", &gameData.camera.zoom, 10, 150);
		ImGui::SliderFloat("Camera speed:", &gameData.cameraSpeed, 10, 150);

		ImGui::EndChild();
		ImGui::Separator();

		ImGui::BeginChild("Player Data");// , ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.3f));

		Vector2 teleportPosition = { 20, 100 };

		ImGui::InputFloat2("Player Position:", &gameData.player.physics.transform.position.x);
		ImGui::InputFloat2("Player Velocity:", &gameData.player.physics.velocity.x);
		ImGui::InputFloat2("Player Acceleration:", &gameData.player.physics.acceleration.x);
		ImGui::InputFloat2("Teleport To:", &teleportPosition.x);

		if (ImGui::Button("Teleport Player"))
		{
			gameData.player.teleport(teleportPosition);
		}

		//ImGui::SliderFloat("Player speed:", &playerSpeed, 10, 150);

		ImGui::EndChild();

		ImGui::Separator();

		// World Data
		ImGui::BeginChild("World Data");// , ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.4f));

		ImGui::InputInt("Seed:", &genSeed, 1, 100);
		ImGui::InputInt("World Width:", &genWorldWidth, 1, 100);
		ImGui::InputInt("World Height:", &genWorldHeight, 1, 100);

		static int texturePackID = -1;
		ImGui::InputInt("Texture Pack ID:", &texturePackID, 1, 2);
		if (ImGui::Button("Load Texture Pack"))
		{
			assetManager.LoadTexturePack(texturePackID);
		}

		if (ImGui::Button("Gravity"))
		{
			gameData.useGravity = !gameData.useGravity;
		}

		if (ImGui::Button("Create New World"))
		{
			gameData.gameMap.seed = genSeed;
			generateWorld(gameData.gameMap, genWorldWidth, genWorldHeight);
		}

		if (ImGui::Button("Copy"))
		{
			gameData.copyStructure.copyFromMap(gameData.gameMap, gameData.selectionStart, gameData.selectionEnd);
		}

		if (ImGui::Button("Spawn Entity"))
		{
			spawnRandomEnemy(gameData.player.getPosition());
		}


		if (ImGui::Button("Hurt Slime"))
		{
			for (auto& e : gameData.entities.entities)
			{
				if (e.second->getEntityType() == EntityType_Slime)
				{
					e.second->OnHit(1, RED);
					break;
				}
			}
		}


		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Save Data");// , ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.5f));

		ImGui::InputText("Save Name", gameData.saveName, sizeof(gameData.saveName));

		if (ImGui::Button("Save To File"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";

			saveBlockDataToFile(gameData.copyStructure.mapData,
				gameData.copyStructure.w, gameData.copyStructure.h,
				path.c_str());
		}
		if (ImGui::Button("Load To File"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";

			loadBlockDataFromFile(gameData.copyStructure.mapData,
				gameData.copyStructure.w, gameData.copyStructure.h,
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
				gameData.creativeSelectedBlock = i;
			}

			ImGui::PopID();

			if (i % 10 != 0)
			{
				ImGui::SameLine();
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

void close_game()
{
	saveSettings();
	gameData = {};

	std::cout << "Close Game \n";
	std::ofstream f(RESOURCES_PATH "debug.log");

	f << "\n CLOSED\n";
	f.close();
}