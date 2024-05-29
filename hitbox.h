#pragma once

struct Hitbox {
    int x, y; // Координаты верхнего левого угла хитбокса
    int width, height; // Размеры хитбокса
};

bool hitboxesIntersect(const Hitbox& h1, const Hitbox& h2);
