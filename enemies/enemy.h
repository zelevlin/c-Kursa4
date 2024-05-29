#pragma once
#include "../player/character.h"
#include "../hitbox.h"
#include <string>
#include <vector>

struct Character;  // Предварительное объявление

struct Enemy {
    int x, y; // Позиция для отрисовки
    int offsetX, offsetY; // Смещение для отрисовки
    int xCenter, yCenter; // Центр спрайта для вычислений
    int width, height; // Размеры спрайта
    Hitbox hitbox;
    int health; // Здоровье
    int damage; // Урон
    bool isAnimating; // Флаг анимации
    bool isAttacking; // Флаг атаки
    std::vector<std::string> spriteLines; // Строки спрайта
    char lastDirection; // Последнее направление движения
    int attackRadius; // Радиус атаки
    std::chrono::time_point<std::chrono::high_resolution_clock> lastAttackTime; // Время последней атаки
    int level;
    bool debugHitbox; //флаг хитбокса противника
    std::chrono::time_point<std::chrono::high_resolution_clock> lastMoveTime;
};


void initEnemy(Enemy& enemy, int x, int y, int health, int damage, std::vector<std::string> spriteLines, int attackRadius, int level);
void drawEnemy(const Enemy& enemy);
void moveEnemy(Enemy& enemy, Character& chara, std::vector<Enemy>& enemies, const std::vector<std::string>& map);
void attackCharacter(Enemy& enemy, Character& chara);
bool isPathClear(int x1, int y1, int x2, int y2, const std::vector<std::string>& map);
bool canAttack(Enemy& enemy, Character& chara, const std::vector<std::string>& map);
void updateEnemies(std::vector<Enemy>& enemies, Character &hero, const std::vector<std::string>& map);
void onMonsterDeath(Enemy& monster, Character& player);