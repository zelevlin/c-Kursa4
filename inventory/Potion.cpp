#include "Potion.h"
#include "../player/character.h"

Potion::Potion(const std::string& name, int level, ItemType type, const std::string& description, const std::string& sprite, int heal)
    : Item{name, type, level, description, sprite}, healAmount(heal) {
    logger.log("Potion created: " + name + " with type " + std::to_string(static_cast<int>(type)));
}

void Potion::use(Character& character) {
    character.health += healAmount; // Предполагаем, что у класса Character есть свойство health
    logger.log("Used " + name + ", healed " + std::to_string(healAmount) + " points."+ std::to_string(level) + " lvl.");
}
