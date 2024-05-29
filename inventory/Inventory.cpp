#include "Inventory.h"
#include <algorithm>
#include <ncurses.h>

Inventory::~Inventory() {
    for (auto item : items) {
        delete item;
    }
    items.clear();
}

void Inventory::addItem(Item* item) {
    items.push_back(item);
}

void Inventory::removeItem(Item* item) {
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        delete *it;
        items.erase(it);
    }
}

Item* Inventory::getItem(const std::string& itemName) {
    auto it = std::find_if(items.begin(), items.end(), [&itemName](const Item* item) {
        return item->name == itemName;
    });
    return it != items.end() ? *it : nullptr;
}

void Inventory::listItems() const {
    clear();
    int row = 0;
    for (const Item* item : items) {
        mvprintw(row++, 0, "Name: %s - %s", item->name.c_str(), item->description.c_str());
    }
    refresh();
}
