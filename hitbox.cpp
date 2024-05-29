#include "hitbox.h"

bool hitboxesIntersect(const Hitbox& h1, const Hitbox& h2) {
    return (h1.x < h2.x + h2.width &&
            h1.x + h1.width > h2.x &&
            h1.y < h2.y + h2.height &&
            h1.y + h1.height > h2.y);
}
