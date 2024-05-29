#pragma once
#include <vector>
#include <ncurses.h>
#include <string>
#include "player/character.h"

void saveScreen(const std::vector<std::string>& levelMap);
std::pair<int, int> updateCamera(int playerX, int playerY, int screenWidth, int screenHeight, const std::vector<std::string>& levelMap);
void drawLevel(int drawStartX, int drawStartY, int screenWidth, int screenHeight);
void restoreScreen(Character& player, const std::vector<std::string>& levelMap);
