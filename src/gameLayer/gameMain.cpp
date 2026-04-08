#include "gameMain.h"

// Add Trees

struct GameData
{
	GameMap gameMap = {};
	Camera2D camera = {};

	int creativeSelectedBlock = Block::dirt;

	Structure copyStructure = {};
	Vector2 selectionStart = {};
	Vector2 selectionEnd = {};

	char saveName[100]{};

	PhysicalEntity player = {};
	Slime slime = {};

}gameData;

AssetManager assetManager;

bool showImgui = false;

bool init_game()
{
	assetManager.loadAll();
	//assetManager.LoadTexturePack(0);

	generateWorld(gameData.gameMap,1000, 500);

	// Camera
	gameData.camera.target = Vector2{ 20, 100 };
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 100.0f;
	// Player update

	gameData.player.transform.position = Vector2{ 20, 100 };
	gameData.player.texture = assetManager.player;
	gameData.player.transform.w = 0.9f;
	gameData.player.transform.h = 1.8f;

	gameData.slime.physics.transform.position = Vector2{ 40, 100 };
	gameData.slime.physics.transform.w = 0.9f;
	gameData.slime.physics.transform.h = 1.8f;

	return true;
}

bool update_game()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	gameData.camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	ClearBackground({ 75, 75, 150, 255 });

	if (IsKeyPressed(KEY_F10)) { showImgui = !showImgui; }

#pragma region Camera Movement
	static float cameraSpeed = 7;
	/*
	if (IsKeyDown(KEY_UP)) gameData.camera.target.y -= cameraSpeed * deltaTime;
	if (IsKeyDown(KEY_DOWN)) gameData.camera.target.y += cameraSpeed * deltaTime;
	if (IsKeyDown(KEY_LEFT)) gameData.camera.target.x -= cameraSpeed * deltaTime;
	if (IsKeyDown(KEY_RIGHT)) gameData.camera.target.x += cameraSpeed * deltaTime;
	*/
	gameData.camera.target = gameData.player.transform.position;

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
			if (blockX <= gameData.player.transform.position.x + createDistance && blockX >= gameData.player.transform.position.x - createDistance &&
				blockY <= gameData.player.transform.position.y + createDistance && blockY >= gameData.player.transform.position.y - createDistance)
			{
				auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
				if (b) *b = {};
			}
			
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			if (blockX <= gameData.player.transform.position.x + createDistance && blockX >= gameData.player.transform.position.x - createDistance &&
				blockY <= gameData.player.transform.position.y + createDistance && blockY >= gameData.player.transform.position.y - createDistance)
			{
				auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
				if (b) b->type = gameData.creativeSelectedBlock;
			}
		}

	}


#pragma endregion


#pragma region Entities
	float playerSpeed = 7;

	if (IsKeyDown(KEY_W)) gameData.player.transform.position.y -= playerSpeed * deltaTime;
	if (IsKeyDown(KEY_S)) gameData.player.transform.position.y += playerSpeed * deltaTime;
	if (IsKeyDown(KEY_A)) gameData.player.transform.position.x -= playerSpeed * deltaTime;
	if (IsKeyDown(KEY_D)) gameData.player.transform.position.x += playerSpeed * deltaTime;
	if (IsKeyDown(KEY_SPACE)) gameData.player.jump(10);

	gameData.player.applyGravity();
	gameData.player.updateForces(deltaTime);
	gameData.player.resolveConstrains(gameData.gameMap);
	gameData.player.updateFinal();

	std::ranlux24_base rng(std::random_device{}());

	gameData.slime.update(deltaTime, rng, gameData.player.getPosition());

	gameData.slime.physics.applyGravity();
	gameData.slime.physics.updateForces(deltaTime);
	gameData.slime.physics.resolveConstrains(gameData.gameMap);
	gameData.slime.physics.updateFinal();

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

#pragma region Entities

	gameData.slime.render(assetManager);

	// Player
	Transform2D playerSprite = gameData.player.transform;
	playerSprite.w = 1;
	playerSprite.h = 2;

	playerSprite.position.y -= (playerSprite.h - gameData.player.transform.h) / 2;

	DrawTexturePro(
		assetManager.player,
		getTextureAtlas(0, 0, 32, 64),
		getRectangeForEntity(gameData.player.transform, 1, 2),
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
		DrawRectangleLinesEx(gameData.player.transform.getAABB(), 0.1f, Color{ 52, 164, 39, 255 });

	}

#pragma endregion 

	EndMode2D();
#pragma region Imgui
	if (showImgui) {

		ImGui::Begin("Game Controller");

		static int genWorldWidth = 900;
		static int genWorldHeight = 500;
		static int genSeed = 0;

		// Camera Data
		ImGui::BeginChild("Camera Data", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.2f));

		ImGui::SliderFloat("Camera zoom:", &gameData.camera.zoom, 10, 150);
		ImGui::SliderFloat("Camera speed:", &cameraSpeed, 10, 150);

		ImGui::EndChild();

		ImGui::BeginChild("Player Data", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.3f));

		Vector2 teleportPosition = { 20, 100 };

		ImGui::InputFloat2("Player Position:", &gameData.player.transform.position.x);
		ImGui::InputFloat2("Player Velocity:", &gameData.player.velocity.x);
		ImGui::InputFloat2("Player Acceleration:", &gameData.player.acceleration.x);
		ImGui::InputFloat2("Teleport To:", &teleportPosition.x);
		
		if (ImGui::Button("Teleport Player"))
		{
			gameData.player.teleport(teleportPosition);
		}

		//ImGui::SliderFloat("Player speed:", &playerSpeed, 10, 150);

		ImGui::EndChild();

		ImGui::Separator();

		// World Data
		ImGui::BeginChild("World Data", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.4f));

		ImGui::InputInt("Seed:", &genSeed, 1, 100);
		ImGui::InputInt("World Width:", &genWorldWidth, 1, 100);
		ImGui::InputInt("World Height:", &genWorldHeight, 1, 100);

		static int texturePackID = -1;

		if (ImGui::Button("Create New World"))
		{
			gameData.gameMap.seed = genSeed;
			generateWorld(gameData.gameMap,genWorldWidth, genWorldHeight);
		}

		if (ImGui::Button("Copy"))
		{
			gameData.copyStructure.copyFromMap(gameData.gameMap, gameData.selectionStart, gameData.selectionEnd);
		}

		ImGui::InputInt("Texture Pack ID:", &texturePackID, 1, 2);
		if (ImGui::Button("Load Texture Pack"))
		{
			assetManager.LoadTexturePack(texturePackID);
		}

		ImGui::EndChild();

		ImGui::BeginChild("Save Data", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.5f));
		
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

		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Creative ", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.6f));

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
				{35, 35}, {atlas.x, atlas.y},
				{atlas.x + atlas.width, atlas.y + atlas.height}))
			{
				gameData.creativeSelectedBlock = i;
			}

			ImGui::PopID();

			if (i % 10 != 0)
			{
				ImGui::SameLine();
			}

		}
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
				{35, 35}, {atlas.x, atlas.y},
				{atlas.x + atlas.width, atlas.y + atlas.height}))
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

		ImGui::End();
	}

#pragma endregion
	DrawFPS(10, 10);

	return true;
}

void close_game()
{

	gameData = {};

	std::cout << "Close Game \n";
	std::ofstream f(RESOURCES_PATH "debug.log");

	f << "\n CLOSED\n";
	f.close();
}