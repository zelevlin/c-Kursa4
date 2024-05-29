#include "Armor.h"
#include "../player/character.h"

Armor::Armor(const std::string& name, const std::string& description, const std::string& sprite, ItemType type, int level, int defense)
    : Item{name, type, level, description, sprite}, defense(defense) {
    logger.log("Armor created: " + name + " with type " + std::to_string(static_cast<int>(type)));
}

void Armor::use(Character& character) {
    switch (type) {
        case ItemType::Helmet:
            if (character.equippedHelmet) {
                character.defense -= character.equippedHelmet->defense; // Убираем старую защиту
            }
            character.equippedHelmet = this;
            break;
        case ItemType::Chestplate:
            if (character.equippedChestplate) {
                character.defense -= character.equippedChestplate->defense; // Убираем старую защиту
            }
            character.equippedChestplate = this;
            break;
        case ItemType::Pants:
            if (character.equippedPants) {
                character.defense -= character.equippedPants->defense; // Убираем старую защиту
            }
            character.equippedPants = this;
            break;
        default:
            return;
    }
    character.defense += defense; // Добавляем новую защиту
    logger.log("Equipped " + name + ", added " + std::to_string(defense) + " defense."+ std::to_string(level) + " lvl.");
}
