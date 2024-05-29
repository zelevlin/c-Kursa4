#pragma once
#include <string>
#include "../logger.h"

enum class ItemType {
    Potion,
    Weapon,
    Helmet,
    Chestplate,
    Pants,
    // другие типы
};

class Item {
public:
    std::string name;
    int level;
    std::string description;
    std::string sprite;
    ItemType type;
    static Logger logger; // Объявляем статический логгер

    // Конструктор объявления
    Item(const std::string& name, ItemType type, int level, const std::string& description, const std::string& sprite);

    virtual void use(class Character& character) = 0;
    virtual ~Item() {}
};
