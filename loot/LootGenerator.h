#pragma once

#include "../inventory/Item.h"

class LootGenerator {
public:
    // Функция для генерации лута из монстра
    static Item* generateLoot(int monsterLevel);

    // Функция для генерации лута из сундука
    static Item* generateChestLoot(int level);
};
