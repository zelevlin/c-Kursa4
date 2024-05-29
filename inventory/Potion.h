#pragma once
#include "Item.h"

class Potion : public Item {
public:
    int healAmount;  // Количество восстанавливаемого здоровья

    // Конструктор
    Potion(const std::string& name, int level, ItemType type, const std::string& description, const std::string& sprite, int heal);

    // Переопределение функции use
    void use(Character& character) override;

    // Деструктор
    virtual ~Potion() {}
};

