#pragma once
#include <vector>
#include "Item.h"

class Inventory {
public:
    Inventory() = default;
    ~Inventory();

    void addItem(Item* item);
    void removeItem(Item* item);  // Изменение здесь
    Item* getItem(const std::string& itemName);
    void listItems() const;  // Для вывода списка предметов с помощью ncurses
    std::vector<Item*> items;
};
