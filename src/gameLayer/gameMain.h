#ifndef GAME_MAIN_H
#define GAME_MAIN_H

#include <iostream>
#include <fstream>
#include <raylib.h>
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>
#include <raymath.h>
#include <randomStuff.h>
#include <string>
#include <limits>

#include <imgui.h>
#include <worldGenerator.h>
#include <structure.h>

bool init_game();
bool update_game();
void close_game();

#endif