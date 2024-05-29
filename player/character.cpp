// character.cpp
#include "character.h"
#include <thread>
#include "../levels/levelManagement/level.h"
#include "inputState.h"
#include "attack.h"
#include "../game/interface.h"

// Конструктор с параметром логгера
Character::Character(const std::string& logFilename) : logger(logFilename) {
    // Остальная инициализация
}

void initCharacter(Character& chara, int x, int y, std::vector<std::string> spriteLines) {
    chara.debugHitboxAttack = false;
    chara.spriteLines = spriteLines;
    chara.height = spriteLines.size();
    chara.width = 0;
    for (const auto& line : spriteLines) {
        if (line.length() > chara.width) chara.width = line.length();
    }
    chara.x = x;
    chara.y = y;
    chara.offsetX = 0;
    chara.offsetY = 0;
    chara.hitbox = {chara.x, chara.y, chara.width, chara.height};
    chara.health = 100;
    chara.damage = 50;
    chara.defense = 0;
    chara.isPunching = false;
    chara.isJumping = false;
    chara.isAnimating = false;
    chara.isInventoryShown = false;
    chara.logger.log("Character spawned at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    
}

void drawCharacter(const Character& chara) {
    for (int i = 0; i < chara.spriteLines.size(); ++i) {
        mvprintw(chara.y - chara.offsetY + i, chara.x - chara.offsetX, "%s", chara.spriteLines[i].c_str());
    }
    refresh();
}
// void drawCharacter(const Character& chara, int levelWidth, int levelHeight) {
//     int max_y, max_x;
//     getmaxyx(stdscr, max_y, max_x);

//     // Половина размера экрана
//     int halfScreenHeight = max_y / 2;
//     int halfScreenWidth = max_x / 2;

//     // Определение смещений относительно видимой области
//     int startX = chara.x - halfScreenWidth;
//     int startY = chara.y - halfScreenHeight;

//     // Корректировка, если персонаж близко к левому или верхнему краю уровня
//     if (startX < 0) startX = 0;
//     if (startY < 0) startY = 0;

//     // Корректировка, если персонаж близко к правому или нижнему краю уровня
//     int endX = startX + max_x;
//     int endY = startY + max_y;
//     if (endX >= levelWidth) startX = levelWidth - max_x;
//     if (endY >= levelHeight) startY = levelHeight - max_y;

//     // Отрисовка персонажа относительно видимой области
//     for (int i = 0; i < chara.spriteLines.size(); ++i) {
//         mvprintw(chara.y + i - startY, chara.x - startX, "%s", chara.spriteLines[i].c_str());
//     }
//     refresh();
// }




void processCharacterMovement(Character &chara, Level &level, const InputState &inputState) {
    bool canMove = true;  // флаг возможности движения

    if (inputState.left) {
        for (int i = 0; i < 3; i++) {
            if (mvinch(chara.y + i - chara.offsetY, chara.x - 1 - chara.offsetX) == '#' || 
                mvinch(chara.y + i - chara.offsetY, chara.x - 1 - chara.offsetX) == '&') {
                canMove = false;
                break;
            }
        }
        if (canMove) {
            chara.x -= 1;
        }
        chara.lastDirection = 'l';
    }
    if (inputState.right) {
        for (int i = 0; i < 3; i++) {
            if (mvinch(chara.y + i - chara.offsetY, chara.x + 3 - chara.offsetX) == '#' || 
                mvinch(chara.y + i - chara.offsetY, chara.x + 3 - chara.offsetX) == '&') {
                canMove = false;
                break;
            }
        }
        if (canMove) {
            chara.x += 1;
        }
        chara.lastDirection = 'r';
    }
    if (inputState.up) {
        for (int i = 0; i < 3; i++) {
            if (mvinch(chara.y - 1 - chara.offsetY, chara.x + i - chara.offsetX) == '#' || 
                mvinch(chara.y - 1 - chara.offsetY, chara.x + i - chara.offsetX) == '&') {
                canMove = false;
                break;
            }
        }
        if (canMove) {
            chara.y -= 1;
        }
        chara.lastDirection = 'u';
    }
    if (inputState.down) {
        for (int i = 0; i < 3; i++) {
            if (mvinch(chara.y + 3 - chara.offsetY, chara.x + i - chara.offsetX) == '#' || 
                mvinch(chara.y + 3 - chara.offsetY, chara.x + i - chara.offsetX) == '&') {
                canMove = false;
                break;
            }
        }
        if (canMove) {
            chara.y += 1;
        }
        chara.lastDirection = 'd';
    }

    if (inputState.attacking && !chara.isPunching && !chara.isAnimating) {
        attack(chara);
        chara.logger.log("Chara is attacking!");
    }
    if (inputState.jumping && !chara.isJumping) {
        performJump(chara);
        chara.logger.log("Chara performed a jump!");
    }
    if (inputState.exit && !inputState.inventory) {
        chara.wantToExit = true;
    }
    if (inputState.inventory) {
        displayInventory(chara);
    }
}


void updateCharacterState(Character &chara) {
    if (chara.isPunching && chara.attackTimer.isExpired()) {
        chara.isPunching = false;
    }
    if (chara.isJumping && chara.jumpTimer.isExpired()) {
        chara.isJumping = false;
    }
    if(chara.isInventoryShown){
        chara.isInventoryShown = false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    chara.isAnimating = false;
}

void Character::updateNearestEnemy(std::vector<Enemy>& enemies) {
    float minDistance = std::numeric_limits<float>::max();
    Enemy* closest = nullptr;

    for (Enemy& enemy : enemies) {
        float distance = std::sqrt(std::pow(this->x - enemy.x, 2) + std::pow(this->y - enemy.y, 2));
        if (distance < minDistance) {
            minDistance = distance;
            closest = &enemy;
        }
    }

    this->nearestEnemy = closest;  // Обновление ближайшего врага
}
