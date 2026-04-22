#include "gameMain.h"

AssetManager assetManager;
GamePlay gamePlay;
UIEngine mainMenuButtons;
DrawBackground backgroundForMainMenu;

bool gamePlayRunning = false;

bool init_game()
{
	Audio::init();
	loadSettings();
	assetManager.loadAll();
	
	gamePlay.init(assetManager);

	return true;
}

static float camX = 0;

bool update_game()
{
	Audio::update();
	updateSettings();

	ClearBackground({ 0, 0, 0,255 });
	if (!gamePlayRunning) {
		Camera2D c = {};
		c.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

		float camSpeed = 0.1f;
		c.zoom = 20;
		camX += camSpeed;
		c.target = Vector2{ camX, 500 };
		backgroundForMainMenu.draw(GetFrameTime(), assetManager, c, { 1000, 1000 });

		addTitle("Terra", mainMenuButtons);

		if (addButton("Start Game", mainMenuButtons))
		{
			std::cout << "Start Game" << "\n";
			gamePlayRunning = true;
		}

		if (addButton("Settings", mainMenuButtons))
		{
			std::cout << "Settings" << "\n";
		}

		if (addButton("Close Game", mainMenuButtons))
		{
			std::cout << "Close Game" << "\n";
			close_game();
		}

		mainMenuButtons.updateAndRender();

		return true;
	}
	else
	{
		return gamePlay.update(assetManager);
	}
	return true;
}

void close_game()
{
	
	saveSettings();

	std::cout << "Close Game \n";
	std::ofstream f(RESOURCES_PATH "debug.log");

	f << "\n CLOSED\n";
	f.close();
	
	CloseWindow();

}