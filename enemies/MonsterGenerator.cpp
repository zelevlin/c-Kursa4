// MonsterGenerator.cpp
#include "MonsterGenerator.h"
#include <fstream>
#include <random>
#include <sstream>

std::vector<std::vector<std::string>> MonsterGenerator::monsterSprites;

void MonsterGenerator::loadMonsterSprites(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open monster sprites file");
    }

    std::string line;
    std::vector<std::string> currentSprite;
    while (std::getline(file, line)) {
        if (line == ",") {
            if (!currentSprite.empty()) {
                monsterSprites.push_back(currentSprite);
                currentSprite.clear();
            }
        } else {
            currentSprite.push_back(line);
        }
    }
    if (!currentSprite.empty()) {
        monsterSprites.push_back(currentSprite);
    }

    file.close();
}

bool MonsterGenerator::canPlaceMonster(const Level& level, int x, int y, const std::vector<std::string>& sprite, const std::vector<Enemy>& enemies) {
    int spriteHeight = sprite.size();
    int spriteWidth = sprite[0].size();

    // Проверка, что спрайт помещается в границы уровня
    if (x + spriteWidth > level.width || y + spriteHeight > level.height) {
        return false;
    }

    // Проверка, что спрайт не накладывается на 'S' или 'E'
    for (int i = 0; i < spriteHeight; ++i) {
        for (int j = 0; j < spriteWidth; ++j) {
            char cell = level.map[y + i][x + j];
            if (cell != ' ' || cell == 'S' || cell == 'E') {
                return false;
            }
        }
    }

    // Проверка, что между монстрами соблюдена минимальная дистанция
    for (const auto& enemy : enemies) {
        int distX = std::abs(enemy.x - x);
        int distY = std::abs(enemy.y - y);
        if (distX < 15 && distY < 15) {
            return false;
        }
    }

    return true;
}

void MonsterGenerator::generateMonsters(Level& level, int levelNumber) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dist(0, monsterSprites.size() - 1); // Генератор случайных чисел для спрайтов

    int maxMonsters = 15; // Максимальное количество монстров на уровне
    int currentMonsters = 0;

    // Проверка, пуст ли вектор монстров, и его очистка, если не пуст
    if (!level.enemies.empty()) {
        level.enemies.clear();
    }

    for (int y = 0; y < level.height && currentMonsters < maxMonsters; ++y) {
        for (int x = 0; x < level.width && currentMonsters < maxMonsters; ++x) {
            // Пропускаем клетки, где уже есть что-то
            if (level.map[y][x] != ' ') continue;

            // Выбираем случайный спрайт из monsterSprites
            const std::vector<std::string>& sprite = monsterSprites[dist(g)];

            // Проверяем, можно ли разместить монстра
            if (canPlaceMonster(level, x, y, sprite, level.enemies)) {
                // Создаем нового монстра и добавляем его в вектор врагов уровня
                Enemy monster;
                initEnemy(monster, x, y, 100, 10, sprite, 1, levelNumber);
                level.enemies.push_back(monster);
                currentMonsters++;
            }
        }
    }
}
