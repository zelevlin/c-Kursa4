#include "Item.h"

// Инициализируем статический логгер
Logger Item::logger("game.log");

// Определение конструктора
Item::Item(const std::string& name, ItemType type, int level, const std::string& description, const std::string& sprite)
    : name(name), type(type), level(level), description(description), sprite(sprite) {
    logger.log("Item created: " + name + " with type " + std::to_string(static_cast<int>(type)));
}
