#pragma once
#ifndef SAVEMAP_H
#define SAVEMAP_H

#include <vector>
#include <blocks.h>
#include <fstream>
#include <asserts.h>
#include <filesystem>
#include <iostream>

bool saveBlockDataToFile(std::vector<Block> blocks, int w, int h, const char* fileName);

bool loadBlockDataFromFile(std::vector<Block> &blocks, int &w, int &h, const char* fileName);

bool writeEntireFile(const char* fileName, const void* data, size_t size);

bool readEntireFile(const char* fileName, const void* data, size_t size);

#endif
