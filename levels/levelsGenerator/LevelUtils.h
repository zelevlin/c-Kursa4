#pragma once
#include <vector>
#include <string>
#include "LevelGenerator.h"

void initialize(char**& dungeon, int width, int height);
void clearDungeon(char** dungeon, int width, int height);
void saveToFile(char** dungeon, int width, int height, const char* baseFilename);
double calculateDistance(int x1, int y1, int x2, int y2);
bool intersects(const LevelGenerator::Room& a, const LevelGenerator::Room& b, int buffer = 1);
bool findEmpty3x3(char** dungeon, int width, int height, int &x, int &y, int excludeX = -1, int excludeY = -1);
void loadRoomTemplates(const std::string& templateDirectory, std::vector<std::vector<std::string>>& roomTemplates);
