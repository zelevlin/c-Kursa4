// MonsterGenerator.h
#pragma once
#include <vector>
#include <string>
#include "enemy.h"
#include "../levels/levelManagement/level.h"

class MonsterGenerator {
public:
    static void loadMonsterSprites(const std::string& filename);
    static void generateMonsters(Level& level, int levelNumber);

private:
    static std::vector<std::vector<std::string>> monsterSprites;
    static bool canPlaceMonster(const Level& level, int x, int y, const std::vector<std::string>& sprite, const std::vector<Enemy>& enemies);
};
