#ifndef GAME_MAIN_H
#define GAME_MAIN_H

#include <iostream>
#include <fstream>
#include <helpers.h>
#include <raymath.h>
#include <string>
#include <limits>
#include <imgui_internal.h>

// Game Layer
#include <developerWindow.h>
#include <randomStuff.h>
#include <raylib.h>
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <imgui.h>
#include <worldGenerator.h>
#include <structure.h>
#include <saveMap.h>
#include <physics.h>
#include <items.h>
#include <audio.h>
#include <settings.h>

// Entities
#include <entity.h>
#include <entities/slime.h>
#include <entities/droppedItem.h>
#include <entities/blueSlime.h>
#include <entities/mummy.h>
#include <entities/zombie.h>
#include "entities/eskimo_zombie.h"
#include <player.h>

bool init_game();
bool update_game();
void close_game();

#endif