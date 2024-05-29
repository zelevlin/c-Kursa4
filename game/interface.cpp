#include "interface.h"
#include "../inventory/Inventory.h"
#include "../inventory/Armor.h"
#include <algorithm>

void drawInventorySlots(const Inventory& inventory, int selectedCategory) {
    int startY = LINES - 3;  // Начинаем рисовать внизу экрана, над строкой подсказок
    int startX = 0;
    int slotWidth = COLS / 5;  // Общая ширина экрана делится на 5 слотов

    const std::vector<std::string> categories = {"Helmet", "Chestplate", "Pants", "Weapon", "Potion"};
    for (int i = 0; i < 5; ++i) {
        if (i == selectedCategory) {
            attron(A_REVERSE);  // Инверсия цвета для выбранного слота
        }
        mvprintw(startY, startX + i * slotWidth, "[%s]", categories[i].c_str());
        if (i == selectedCategory) {
            attroff(A_REVERSE);
        }
    }
}

std::vector<Item*> filterItemsByCategory(const Inventory& inventory, ItemType type) {
    std::vector<Item*> filteredItems;
    for (auto item : inventory.items) {
        if (item->type == type) {
            filteredItems.push_back(item);
        }
    }
    return filteredItems;
}

void displayStats(const Character& chara) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int statStartY = max_y - 1;  // Последняя строка окна
    int statStartX = 0;          // Начало строки

    attron(A_REVERSE); // Включаем инвертирование цвета
    mvprintw(statStartY, statStartX, "Health: %d, Damage: %d, Defense: %d, X: %d, Y: %d ",
             chara.health, chara.damage, chara.defense, chara.x, chara.y);
    attroff(A_REVERSE); // Выключаем инвертирование цвета
    refresh();
}



void displayInventory(Character& character) {
    Inventory& inventory = character.inventory;
    int selectedCategory = 4;  // Начинаем с категории Potion
    int selectedIndex = 0;  // Индекс выбранного предмета
    bool inInventory = true;

    std::vector<ItemType> categories = {ItemType::Helmet, ItemType::Chestplate, ItemType::Pants, ItemType::Weapon, ItemType::Potion};

    while (inInventory) {
        clear();  // Очистка экрана перед выводом инвентаря
        drawInventorySlots(inventory, selectedCategory);

        std::vector<Item*> filteredItems = filterItemsByCategory(inventory, categories[selectedCategory]);

        int row = 0;
        if (filteredItems.empty()) {
            mvprintw(row, 0, "Empty");
        } else {
            for (int i = 0; i < filteredItems.size(); ++i) {
                if (i == selectedIndex) {
                    attron(A_REVERSE);  // Выделение выбранного предмета
                }
                mvprintw(row, 0, "Name: %s - %s (Level: %d)", filteredItems[i]->name.c_str(), filteredItems[i]->description.c_str(), filteredItems[i]->level);
                //mvprintw(row, 60, "Type: %d", static_cast<int>(filteredItems[i]->type)); // Отладочное сообщение
                if (i == selectedIndex) {
                    attroff(A_REVERSE);
                }
                row++;
            }
        }
        mvprintw(LINES - 1, 0, "Use [LEFT]/[RIGHT] to change category, [UP]/[DOWN] to navigate, [ENTER] to use item, [Q] to quit.");
        refresh();

        int ch = getch();  // Получение ввода от пользователя
        switch (ch) {
            case KEY_UP:
                if (selectedIndex > 0) selectedIndex--;
                break;
            case KEY_DOWN:
                if (selectedIndex < filteredItems.size() - 1) selectedIndex++;
                break;
            case KEY_LEFT:
                if (selectedCategory > 0) {
                    selectedCategory--;
                    selectedIndex = 0; // Сбросить индекс при смене категории
                }
                break;
            case KEY_RIGHT:
                if (selectedCategory < categories.size() - 1) {
                    selectedCategory++;
                    selectedIndex = 0; // Сбросить индекс при смене категории
                }
                break;
            case 'q':  // Выход из инвентаря
            case 'Q':
                inInventory = false;
                break;
            case '\n':  // Действие с выбранным предметом, например, использование
                if (filteredItems.size() > selectedIndex) {
                    Item* selectedItem = filteredItems[selectedIndex];
                    selectedItem->use(character);
                    inventory.removeItem(selectedItem);  // Удаление предмета из инвентаря
                    if (selectedIndex >= filteredItems.size()) {
                        selectedIndex = std::max(0, static_cast<int>(filteredItems.size()) - 1);
                    }
                    displayStats(character);  // Обновить отображение статистики после использования предмета
                }
                break;
        }
    }
}

void displayMainMenu() {
    clear(); // Очистка экрана перед выводом меню
    mvprintw(10, 10, "Main Menu:");
    mvprintw(12, 10, "1. Start Infinity Game");
    mvprintw(14, 10, "2. Exit");
    // mvprintw(16, 10, "3. Level generator debugging");
    // mvprintw(18, 10, "4. Infinity mode");
    refresh();
}
