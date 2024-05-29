#pragma once
#include "../player/character.h"
#include <ncurses.h>

// Функция для отображения статистики и характеристик персонажа
void displayStats(const Character& chara);

// Функция для отображения инвентаря
void displayInventory(Character& character);

// Функция для отображения главного меню
void displayMainMenu();

// Функция для отрисовки слотов инвентаря
void drawInventorySlots(const Inventory& inventory, int selectedCategory);

// Функция для фильтрации предметов по категориям
std::vector<Item*> filterItemsByCategory(const Inventory& inventory, ItemType type);
