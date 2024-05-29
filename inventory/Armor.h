#pragma once
#include "Item.h"

class Armor : public Item {
public:
    int defense;
    Armor(const std::string& name, const std::string& description, const std::string& sprite, ItemType type, int level, int defense);
    void use(Character& character) override;
};
