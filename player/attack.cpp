#include "attack.h"
#include "character.h"
#include "enemy.h"
#include "hitbox.h"

void calcAttack(Character& chara, Enemy& enemy) {
    // Предполагаем, что хитбокс атаки смещен вправо от персонажа
    int attackWidth=1, attackHeight=1; //заглушка потом добавить оружие
    Hitbox attackHitbox = {chara.hitbox.x, chara.hitbox.y,
     attackWidth, attackHeight};

    // Проверяем пересечение хитбокса атаки и врага
    if (intersect(attackHitbox, enemy.hitbox)) {
        enemy.health -= chara.damage;
        // Логика визуализации попадания и анимации удара...
    }
}

bool intersect(const Hitbox& hb1, const Hitbox& hb2) {
    // Проверяем, не находится ли один хитбокс целиком слева/справа/сверху/снизу от другого
    return !(hb1.x + hb1.width < hb2.x || hb1.x > hb2.x + hb2.width ||
             hb1.y + hb1.height < hb2.y || hb1.y > hb2.y + hb2.height);
}


//подумать над классом для атак героя и противника, запихнуть туда все атаки, 
//а в отдельном классе обрабатывать их