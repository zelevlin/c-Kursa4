#pragma once
#include <vector>
#include <string>
#include "../../enemies/enemy.h"
struct Enemy;  // Предварительное объявление

struct Level {
    int width;
    int height;
    int startX;  // Начальная позиция X для персонажа
    int startY;  // Начальная позиция Y для персонажа
    int endX; // Конечная позиция X для персонажа
    int endY; // Конечная позиция Y для персонажа
    std::vector<std::string> map;  // Добавлено для хранения карты уровня
    std::vector<Enemy> enemies; // Вектор для хранения врагов

    // Конструктор для инициализации значений по умолчанию
};


