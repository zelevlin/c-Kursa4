#pragma once
#include "Item.h"

class Weapon : public Item {
public:
    int damage;
    float attackSpeed;
    Weapon(const std::string& name, int level, const std::string& description, const std::string& sprite, int dmg, float speed);
    void use(class Character& character) override;
};
