#pragma once
#include <string>
#include <vector>
#include <ncurses.h>
#include "../levels/levelManagement/level.h"
#include "inputState.h"
#include "../hitbox.h"
#include "../inventory/Inventory.h"
#include "../inventory/Armor.h"
#include "../inventory/Weapon.h"
#include "../logger.h"
#include "../timer.h"  // Включаем заголовок для таймеров

class Enemy;  // Предварительное объявление
struct Level;
struct Character {
    int x, y; // позиция для отрисовки
    int offsetX, offsetY; // смещение для отрисовки
    int width, height; // Размеры спрайта
    Hitbox hitbox;
    int health;
    int damage;
    int defense; // Показатель защиты
    std::vector<std::string> spriteLines; // Строки, формирующие спрайт
    


    InputState state;  // Использование InputState в структуре Character
    char lastDirection; // направление
    Enemy* nearestEnemy = nullptr; // Указатель на ближайшего врага
    char lastDirectionToAttack; // автоприцел
    bool isAnimating;  // Флаг активной анимации
    bool isPunching;
    bool isJumping;
    bool isInventoryShown;
    Inventory inventory;
    bool debugHitboxAttack;
    bool wantToExit;
    
    Logger logger;  // Объект логгера как член класса
    Timer attackTimer;  // Таймер для атаки
    Timer jumpTimer;  // Таймер для прыжка

    // Экипированные предметы брони
    Armor* equippedHelmet = nullptr;
    Armor* equippedChestplate = nullptr;
    Armor* equippedPants = nullptr;
    Weapon* equippedWeapon = nullptr;

    void updateNearestEnemy(std::vector<Enemy>& enemies); // Объявление функции
    Character(const std::string& logFilename);  // Конструктор с параметром логгера
};

void attack(Character &chara);
void performJump(Character &chara);

void initCharacter(Character& chara, int x, int y, std::vector<std::string> spriteLines);
void drawCharacter(const Character& chara);
void updateCharacterState(Character &chara);
void processCharacterMovement(Character &chara, Level &level, const InputState &inputState);
