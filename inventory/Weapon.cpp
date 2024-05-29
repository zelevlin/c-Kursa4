#include "Weapon.h"
#include "../player/character.h"

Weapon::Weapon(const std::string& name, int level, const std::string& description, const std::string& sprite, int dmg, float speed)
    : Item{name, ItemType::Weapon, level, description, sprite}, damage(dmg), attackSpeed(speed) {
    logger.log("Weapon created: " + name + " with level " + std::to_string(level));
}

void Weapon::use(Character& character) {
    

    if (character.equippedWeapon) {
            character.damage -= character.equippedWeapon->damage; // Убираем старую защиту
    }
    character.equippedWeapon = this;
    character.damage += damage; // Простое действие для примера
    logger.log("Equipped weapon: " + name + ", added " + std::to_string(damage) + " damage."+ std::to_string(level) + " lvl.");
}
