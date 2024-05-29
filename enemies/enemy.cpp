#include "enemy.h"
#include <ncurses.h>
#include <cmath>
#include "../player/character.h"
#include "../loot/LootGenerator.h"
#include "../loot/LootGenerator.h"

void initEnemy(Enemy& enemy, int x, int y, int health, int damage, std::vector<std::string> spriteLines, int attackRadius, int level) {
    enemy.debugHitbox = false;
    enemy.spriteLines = spriteLines;
    enemy.height = spriteLines.size();
    enemy.width = 0;
    for (const auto& line : spriteLines) {
        if (line.length() > enemy.width) enemy.width = line.length();
    }
    enemy.x = x;
    //enemy.xCenter = x - floor(enemy.width / 2);
    enemy.y = y;
    enemy.offsetX = 0;
    enemy.offsetY = 0;
   // enemy.yCenter = y - floor(enemy.height / 2);
    //enemy.health = 100; // Пример значений
    enemy.hitbox = {enemy.x, enemy.y, enemy.width, enemy.height};
    enemy.damage = damage;
    enemy.health = health;
    enemy.isAnimating = false;
    enemy.isAttacking = false;
    enemy.lastDirection = 'l';
    enemy.attackRadius = attackRadius;
    enemy.lastAttackTime = std::chrono::high_resolution_clock::now(); // Начальное время
    enemy.level = 1;
    enemy.lastMoveTime = std::chrono::high_resolution_clock::now();
}

void updateEnemies( std::vector<Enemy>& enemies, Character &hero, const std::vector<std::string>& map){
    auto it = enemies.begin();
    while (it != enemies.end()) {
        moveEnemy(*it, hero,enemies, map);
        attackCharacter(*it, hero);

        if (it->health <= 0) {
            // Очистить позицию врага на экране
            for (int i = 0; i < it->height; ++i) {
                move(it->y + i, it->x); // Перемещаем курсор на начало каждой строки спрайта врага
                clrtoeol(); // Очищаем строку
            }

            // Удаление врага из списка
            onMonsterDeath(*it, hero); //выпадение лута из монстра
            it = enemies.erase(it); // Это также перемещает итератор на следующий элемент
            
            
        } else {
            drawEnemy(*it);
            ++it; // Переход к следующему врагу
        }
    }

}

void onMonsterDeath(Enemy& monster, Character& player) {
    Item* loot = LootGenerator::generateLoot(monster.level);
    if (loot != nullptr) {
        player.inventory.addItem(loot);
    }
}

void drawEnemy(const Enemy& enemy) {
    for (int i = 0; i < enemy.spriteLines.size(); ++i) {
        mvprintw(enemy.y - enemy.offsetY + i, enemy.x - enemy.offsetX, "%s", enemy.spriteLines[i].c_str());

    }
    mvprintw(enemy.y - enemy.offsetY - 1, enemy.x - enemy.offsetX, "%d", enemy.health);
    //mvprintw(enemy.y - enemy.offsetY - 1, enemy.x - enemy.offsetX, "%d, %d", enemy.health, enemy.level);
    //mvprintw(4, 32, "enemy height: %d", enemy.height);
    //mvprintw(5, 32, "enemy width: %d", enemy.width);

    if (enemy.debugHitbox){

    
    // Отрисовка хитбокса вокруг врага
    int top = enemy.y;
    int bottom = enemy.y + enemy.height - 1;
    int left = enemy.x;
    int right = enemy.x + enemy.width - 1;

    // Верхняя и нижняя границы
    for (int x = left; x <= right; x++) {
        mvaddch(top - 1, x, '-'); // верхняя граница
        mvaddch(bottom + 1, x, '-'); // нижняя граница
    }

    // Левая и правая границы
    for (int y = top; y <= bottom; y++) {
        mvaddch(y, left - 1, '|'); // левая граница
        mvaddch(y, right + 1, '|'); // правая граница
    }

    // Угловые символы для красоты
    mvaddch(top - 1, left - 1, '+'); // верхний левый угол
    mvaddch(top - 1, right + 1, '+'); // верхний правый угол
    mvaddch(bottom + 1, left - 1, '+'); // нижний левый угол
    mvaddch(bottom + 1, right + 1, '+'); // нижний правый угол
    }
    refresh();
}

bool isPathClear(int x1, int y1, int x2, int y2, const std::vector<std::string>& map) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        // Проверка на наличие символа препятствия '#'
        if (map[y1][x1] == '&' || map[y1][x1] == '|') return false; 

        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
    return true; // Путь свободен
}


void moveEnemy(Enemy& enemy, Character& chara, std::vector<Enemy>& enemies, const std::vector<std::string>& map) {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - enemy.lastMoveTime;

    if (elapsed.count() >= 0.3f) { // Проверяем, прошло ли 0.5 секунды
        float distance = std::sqrt(std::pow(chara.x - enemy.x, 2) + std::pow(chara.y - enemy.y, 2));

        if (isPathClear(enemy.x, enemy.y, chara.x, chara.y, map)) { // Проверяем, виден ли герой
            // Определяем направления движения врага
            int dx = (chara.x > enemy.x) ? 1 : (chara.x < enemy.x) ? -1 : 0;
            int dy = (chara.y >= enemy.y) ? 1 : (chara.y < enemy.y) ? -1 : 0;

            // Проверяем, можно ли двигаться в эти направления, учитывая размер спрайта врага
            bool canMoveX = true, canMoveY = true;

            // Предварительно вычислим потенциальные новые координаты
            int newX = enemy.x + dx;
            int newY = enemy.y + dy;

            for (const auto& other : enemies) {
                if (&other != &enemy) {  // Исключаем самого себя из проверки
                    if (hitboxesIntersect({newX, enemy.y, enemy.width, enemy.height}, other.hitbox)) {
                        canMoveX = false;
                    }
                    if (hitboxesIntersect({enemy.x, newY, enemy.width, enemy.height}, other.hitbox)) {
                        canMoveY = false;
                    }
                }
            }

            // Движение врага, если это возможно
            if (dx != 0 && canMoveX && distance - 1 > enemy.attackRadius) {
                enemy.x += dx;
            }
            if (dy != 0 && canMoveY && distance - 1 > enemy.attackRadius) {
                enemy.y += dy;
            }

            // Обновление хитбокса врага после изменения позиции
            enemy.hitbox.x = enemy.x;
            enemy.hitbox.y = enemy.y;
        }

        // Обновление времени последнего движения
        enemy.lastMoveTime = now;
    }

    // Обработка атаки
    float distance = std::sqrt(std::pow(chara.x - enemy.x, 2) + std::pow(chara.y - enemy.y, 2));
    if (distance <= enemy.attackRadius && canAttack(enemy, chara, map) &&
        std::chrono::duration_cast<std::chrono::seconds>(now - enemy.lastAttackTime).count() > 2) {
        enemy.isAttacking = true;
        attackCharacter(enemy, chara);
        enemy.lastAttackTime = now;
    }
}


bool canAttack(Enemy& enemy, Character& chara, const std::vector<std::string>& map) {
    // Проверяем, есть ли препятствия между врагом и героем на линии атаки
    return isPathClear(enemy.x, enemy.y, chara.x, chara.y, map);
}


void attackCharacter(Enemy& enemy, Character& chara) {
    // if (abs(enemy.x - chara.x) < 2 && abs(enemy.y - chara.y) < 2) {
    //     chara.health -= enemy.damage;
    //     enemy.isAnimating = true;
    // }
    if (enemy.isAttacking) {
        int dealingDamage = std::max(0, enemy.damage - chara.defense);
        chara.health -= dealingDamage;
        enemy.isAttacking = false;
    }

}


