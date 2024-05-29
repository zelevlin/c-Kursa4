#include "character.h"
#include "../enemies/enemy.h"

struct Attack {
    int damage; // Величина урона
    int range; // Дальность атаки
    int xHit, yHit; // Координаты места удара
};

void calcAttack(Character& chara, Enemy& enemy);
bool intersect(const Hitbox& hero, const Hitbox& enemy);