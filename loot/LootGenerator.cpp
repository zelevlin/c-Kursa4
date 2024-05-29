
#include "../inventory/Potion.h"
#include "../inventory/Weapon.h"
#include "../inventory/Armor.h"
#include "LootGenerator.h"
#include <random>

// Функция для генерации случайного числа в диапазоне [min, max]
int getRandomNumber(int min, int max) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(min, max); // define the range
    return distr(gen);
}

Item* LootGenerator::generateLoot(int monsterLevel) {
    // Понижаем шанс выпадения до 5%
    if (getRandomNumber(1, 100) > 15) {
        return nullptr; // Ничего не выпадает
    }

    int itemTypeRoll = getRandomNumber(1, 100);
    int itemLevel = monsterLevel;

    if (itemTypeRoll <= 30) { // 30% шанс на зелье
        int potionTypeRoll = getRandomNumber(1, 100);
        if (potionTypeRoll <= 60) {
            return new Potion("Health Potion", itemLevel, ItemType::Potion, "Restores 50 HP", "[P]", 50);
        } else if (potionTypeRoll <= 90) {
            return new Potion("Strong Health Potion", itemLevel, ItemType::Potion, "Restores 100 HP", "[P]", 100);
        } else {
            return new Potion("Mega Health Potion", itemLevel, ItemType::Potion, "Restores 200 HP", "[P]", 200);
        }
    } else if (itemTypeRoll <= 60) { // 30% шанс на броню
        int armorTypeRoll = getRandomNumber(1, 100);
        if (armorTypeRoll <= 33) {
            return new Armor("Basic Helmet", "Provides minimal protection", "[H]", ItemType::Helmet, itemLevel, 5);
        } else if (armorTypeRoll <= 66) {
            return new Armor("Basic Chestplate", "Provides moderate protection", "[C]", ItemType::Chestplate, itemLevel, 10);
        } else {
            return new Armor("Basic Pants", "Provides light protection", "[P]", ItemType::Pants, itemLevel, 5);
        }
    } else { // 40% шанс на оружие
        int weaponTypeRoll = getRandomNumber(1, 100);
        if (weaponTypeRoll <= 50) {
            return new Weapon("Sword", itemLevel, "Deals damage", "[W]", itemLevel * 7, 1.0f);
        } else {
            return new Weapon("Magic Sword", itemLevel, "Deals more damage", "[W]", itemLevel * 7, 1.5f);
        }
    }
}

Item* LootGenerator::generateChestLoot(int currentLevel) {
    int itemTypeRoll = getRandomNumber(1, 100);
    int itemLevel = currentLevel + getRandomNumber(1, 3); // Уровень предмета в сундуке на 1-3 уровня выше

    if (itemTypeRoll <= 30) { // 30% шанс на зелье
        int potionTypeRoll = getRandomNumber(1, 100);
        if (potionTypeRoll <= 60) {
            return new Potion("Health Potion", itemLevel, ItemType::Potion, "Restores 50 HP", "[P]", 50);
        } else if (potionTypeRoll <= 90) {
            return new Potion("Strong Health Potion", itemLevel, ItemType::Potion, "Restores 100 HP", "[P]", 100);
        } else {
            return new Potion("Mega Health Potion", itemLevel, ItemType::Potion, "Restores 200 HP", "[P]", 200);
        }
    } else if (itemTypeRoll <= 60) { // 30% шанс на броню
        int armorTypeRoll = getRandomNumber(1, 100);
        if (armorTypeRoll <= 33) {
            return new Armor("Basic Helmet", "Provides minimal protection", "[H]", ItemType::Helmet, itemLevel, 5);
        } else if (armorTypeRoll <= 66) {
            return new Armor("Basic Chestplate", "Provides moderate protection", "[C]", ItemType::Chestplate, itemLevel, 10);
        } else {
            return new Armor("Basic Pants", "Provides light protection", "[P]", ItemType::Pants, itemLevel, 5);
        }
    } else { // 40% шанс на оружие
        int weaponTypeRoll = getRandomNumber(1, 100);
        if (weaponTypeRoll <= 50) {
            return new Weapon("Sword", itemLevel, "Deals damage", "[W]", itemLevel * 7, 1.0f);
        } else {
            return new Weapon("Magic Sword", itemLevel, "Deals more damage", "[W]", itemLevel * 7, 1.5f);
        }
    }
}
