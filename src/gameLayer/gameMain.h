#ifndef GAME_MAIN_H
#define GAME_MAIN_H

#include <iostream>
#include <fstream>
#include <helpers.h>
#include <raymath.h>
#include <string>
#include <limits>

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
#include <structure.h>
#include <entities/slime.h>

bool init_game();
bool update_game();
void close_game();

#endif