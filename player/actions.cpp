// actions.cpp
#include "character.h"
#include "ncurses.h"
#include "../enemies/enemy.h"
#include "../hitbox.h"
#include <chrono>
#include "../timer.h"



void attack(Character &chara) {
    if (chara.nearestEnemy == nullptr || chara.isPunching) {
        chara.logger.log("No enemy"); 
        return;
    }

    chara.isPunching = true;
    chara.attackTimer.set(std::chrono::milliseconds(100));  // Устанавливаем таймер на 100 мс
    if (std::abs(chara.x - chara.nearestEnemy->x) > std::abs(chara.y - chara.nearestEnemy->y)) {
        chara.lastDirectionToAttack = (chara.x < chara.nearestEnemy->x) ? 'r' : 'l'; // Враг справа или слева
    } else {
        chara.lastDirectionToAttack = (chara.y < chara.nearestEnemy->y) ? 'd' : 'u'; // Враг снизу или сверху
    }

    // Определение хитбокса персонажа для атаки
    Hitbox attackBox = {chara.x, chara.y, 3, 2}; // Предполагается, что у Character есть width и height

    if (chara.lastDirectionToAttack == 'l') {
        mvprintw(chara.y + 1 - chara.offsetY, chara.x - 1 - chara.offsetX, "x~|\\");
        attackBox = {chara.x - 3, chara.y + 1, 3, 1};
    } else if (chara.lastDirectionToAttack == 'r') {
        mvprintw(chara.y + 1 - chara.offsetY, chara.x - chara.offsetX, "/|~x");
        attackBox = {chara.x + 2, chara.y + 1, 3, 1};
    } else if (chara.lastDirectionToAttack == 'u') {
        mvprintw(chara.y + 1 - chara.offsetY, chara.x - chara.offsetX, "/||");
        mvprintw(chara.y - 2 - chara.offsetY, chara.x + 1 - chara.offsetX, "X");
        attackBox = {chara.x, chara.y - 1, 3, 1};
    } else if (chara.lastDirectionToAttack == 'd') {
        mvprintw(chara.y + 1 - chara.offsetY, chara.x - chara.offsetX, "/||");
        mvprintw(chara.y + 3 - chara.offsetY, chara.x + 1 - chara.offsetX, "X");
        attackBox = {chara.x, chara.y + 3, 3, 1};
    }

    if (chara.debugHitboxAttack) {
        // Вывод хитбокса атаки для отладки
        int left = attackBox.x;
        int right = attackBox.x + attackBox.width - 1;
        int top = attackBox.y;
        int bottom = attackBox.y + attackBox.height - 1;

        for (int x = left; x <= right; x++) {
            mvaddch(top - 1 - chara.offsetY, x - chara.offsetX, '-'); // верхняя граница
            mvaddch(bottom + 1 - chara.offsetY, x - chara.offsetX, '-'); // нижняя граница
        }
        for (int y = top; y <= bottom; y++) {
            mvaddch(y - chara.offsetY, left - 1 - chara.offsetX, '|'); // левая граница
            mvaddch(y - chara.offsetY, right + 1 - chara.offsetX, '|'); // правая граница
        }
    }

    // Проверка попадания
    if (hitboxesIntersect(attackBox, chara.nearestEnemy->hitbox)) {
        chara.nearestEnemy->health -= chara.damage; // Нанесение урона
    }

    refresh();
    chara.logger.log("Chara is attacking!");  // Логгирование атаки

    drawCharacter(chara); // Возвращение к базовой отрисовке
    chara.isPunching = false;  // Важно! Сброс состояния атаки
    flushinp();  // Очистить весь ввод, который произошёл до начала анимации
}


void performJump(Character &chara) {
    if (chara.isJumping) return;

    chara.isJumping = true;
    chara.jumpTimer.set(std::chrono::milliseconds(200));  // Устанавливаем таймер на 200 мс

    int verticalJumpHeight = 3;
    int horizontalJumpDistance = 3;

    int dx = 0, dy = 0;
    switch (chara.lastDirection) {
        case 'l': dx = -horizontalJumpDistance; break;
        case 'r': dx = horizontalJumpDistance; break;
        case 'u': dy = -verticalJumpHeight; break;
        case 'd': dy = verticalJumpHeight; break;
    }

    for (int i = 0; i < verticalJumpHeight; ++i) {
        chara.x += dx / verticalJumpHeight;
        chara.y += dy / verticalJumpHeight;
    }

    drawCharacter(chara);
    chara.logger.log("Chara performed a jump!");  // Логгирование прыжка
}
