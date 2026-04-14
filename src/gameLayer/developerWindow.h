#pragma once

#ifndef DEVELOPERWINDOW_H
#define DEVELOPERWINDOW_H

#include <raylib.h>
#include <imgui.h>
#include <memory>
#include <vector>
#include <assetManager.h>

struct GameData;

struct Window
{
	AssetManager assetManager;

	static int windowWidth;
	static int windowHeight;
	bool enabled;

	void render(GameData data );

};

class DeveloperWindow
{
private:

public:
	bool camWindow = false;
	bool playerWindow = false;
	bool worldWindow = false;
	bool creativeWindow = false;
	bool entityWindow = false;
	bool saveWindow = false;

	std::vector<Window> windows = {};

	void render(GameData data);

};


#endif
