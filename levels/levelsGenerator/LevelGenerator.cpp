#include "LevelGenerator.h"
#include "LevelUtils.h"
#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream> // Для отладочных сообщений

int LevelGenerator::WIDTH = 100;
int LevelGenerator::HEIGHT = 100;
char** LevelGenerator::dungeon = nullptr;
std::vector<LevelGenerator::Room> LevelGenerator::rooms;
std::vector<std::vector<std::string>> LevelGenerator::roomTemplates;
Logger LevelGenerator::logger("gen.log"); // Инициализация статического логгера


int LevelGenerator::minRooms = 2; // Инициализация минимального количества комнат
int LevelGenerator::maxRooms = 15; // Инициализация максимального количества комнат


void LevelGenerator::setRoomCountRange(int minR, int maxR) {
    minRooms = minR;
    maxRooms = maxR;
}

int LevelGenerator::getRandomRoomCount() {
    return rand() % (maxRooms - minRooms + 1) + minRooms;
}

void LevelGenerator::generateRooms() {
    int maxAttempts = 100; // Увеличиваем количество попыток размещения каждой комнаты
    int maxRetryAttempts = 10; // Ограничение на количество повторных попыток для каждой комнаты
    int buffer = 2; // Увеличиваем буфер для разделения комнат
    rooms.clear(); // Очищаем вектор комнат перед началом генерации

    std::random_device rd; // Создаем случайное устройство для инициализации генератора случайных чисел
    std::mt19937 g(rd()); // Инициализируем Mersenne Twister генератор случайных чисел

    int roomCount = getRandomRoomCount(); // Получаем случайное количество комнат в пределах заданного диапазона
    logger.log("Generating " + std::to_string(roomCount) + " room(s)");

    for (int i = 0; i < roomCount; i++) { // Пытаемся разместить заданное количество комнат
        bool roomAdded = false; // Флаг, который указывает, была ли добавлена комната
        int retryAttempts = 0; // Счетчик повторных попыток

        while (!roomAdded && retryAttempts < maxRetryAttempts) { // Делаем несколько попыток для размещения каждой комнаты
            for (int attempt = 0; attempt < maxAttempts; attempt++) { // Делаем несколько попыток для размещения каждой комнаты
                int templateIndex = rand() % roomTemplates.size(); // Случайным образом выбираем шаблон комнаты
                const std::vector<std::string>& roomTemplate = roomTemplates[templateIndex]; // Получаем шаблон комнаты по индексу
                int roomHeight = roomTemplate.size(); // Высота комнаты равна числу строк в шаблоне
                int roomWidth = roomTemplate[0].size(); // Ширина комнаты равна числу символов в первой строке шаблона

                // Генерируем случайные координаты для верхнего левого угла комнаты с учетом буферной зоны
                int roomX = rand() % (WIDTH - roomWidth - 2 * buffer) + buffer;
                int roomY = rand() % (HEIGHT - roomHeight - 2 * buffer) + buffer;

                Room newRoom(roomX, roomY, roomWidth, roomHeight); // Создаем новый объект комнаты с заданными параметрами
                bool intersecting = false; // Флаг, который указывает, пересекается ли новая комната с существующими

                // Проверяем, пересекается ли новая комната с уже существующими
                for (const auto& room : rooms) {
                    if (intersects(newRoom, room, buffer)) { // Если комнаты пересекаются
                        intersecting = true; // Устанавливаем флаг пересечения
                        break; // Прерываем цикл проверки
                    }
                }

                if (!intersecting) { // Если комната не пересекается с другими
                    rooms.push_back(newRoom); // Добавляем новую комнату в список комнат
                    logger.log("Added room at (" + std::to_string(roomX) + ", " + std::to_string(roomY) + ") with size (" + std::to_string(roomWidth) + "x" + std::to_string(roomHeight) + ")");
                                        // Заполняем соответствующую область в подземелье символами из шаблона комнаты
                    for (int y = 0; y < roomHeight; y++) {
                        for (int x = 0; x < roomWidth; x++) {
                            dungeon[roomY + y][roomX + x] = roomTemplate[y][x];
                        }
                    }
                    roomAdded = true; // Устанавливаем флаг, что комната была добавлена
                    break; // Прерываем цикл попыток, так как комната успешно добавлена
                }
            }

            if (!roomAdded) { // Если после всех попыток комната не была добавлена
                retryAttempts++; // Увеличиваем счетчик повторных попыток
                logger.log("Retrying room " + std::to_string(i + 1) + ", attempt " + std::to_string(retryAttempts));
            }
        }

        if (!roomAdded && retryAttempts >= maxRetryAttempts) {
            logger.log("Failed to place room " + std::to_string(i + 1) + " after " + std::to_string(maxRetryAttempts) + " retry attempts. Skipping this room.");
        }
    }

     logger.log("Total rooms generated: " + std::to_string(rooms.size()));
}


void LevelGenerator::connectRooms() {
    logger.log("Connecting rooms...");
    int corridorWidth = 5; // Ширина коридора

    auto createCorridor = [&](int x1, int y1, int x2, int y2) {
        logger.log("Creating corridor from (" + std::to_string(x1) + ", " + std::to_string(y1) + ") to (" + std::to_string(x2) + ", " + std::to_string(y2) + ")");
        if (x1 != x2) {
            int corridorXStart = std::min(x1, x2);
            int corridorXEnd = std::max(x1, x2);
            int corridorY = y1;

            for (int x = corridorXStart; x <= corridorXEnd; ++x) {
                for (int y = corridorY - corridorWidth / 2; y <= corridorY + corridorWidth / 2; ++y) {
                    if (dungeon[y][x] != '#') {
                        dungeon[y][x] = ' ';
                    }
                }
            }
        }

        if (y1 != y2) {
            int corridorYStart = std::min(y1, y2);
            int corridorYEnd = std::max(y1, y2);
            int corridorX = x2;

            for (int y = corridorYStart; y <= corridorYEnd; ++y) {
                for (int x = corridorX - corridorWidth / 2; x <= corridorX + corridorWidth / 2; ++x) {
                    if (dungeon[y][x] != '#') {
                        dungeon[y][x] = ' ';
                    }
                }
            }
        }
    };

    auto adjustCorridorStartEnd = [&](int& x, int& y, const Room& room) {
        if (x < room.x) x = room.x - 1;
        else if (x >= room.x + room.width) x = room.x + room.width;

        if (y < room.y) y = room.y - 1;
        else if (y >= room.y + room.height) y = room.y + room.height;
    };

    for (size_t i = 1; i < rooms.size(); ++i) {
        Room& roomA = rooms[i - 1];
        Room& roomB = rooms[i];

        // Находим центр каждой комнаты
        int centerAX = roomA.x + roomA.width / 2;
        int centerAY = roomA.y + roomA.height / 2;
        int centerBX = roomB.x + roomB.width / 2;
        int centerBY = roomB.y + roomB.height / 2;

        adjustCorridorStartEnd(centerAX, centerAY, roomA);
        adjustCorridorStartEnd(centerBX, centerBY, roomB);

        logger.log("Connecting room " + std::to_string(i - 1) + " at (" + std::to_string(centerAX) + ", " + std::to_string(centerAY) + ") to room " + std::to_string(i) + " at (" + std::to_string(centerBX) + ", " + std::to_string(centerBY) + ")");

        bool corridorConnected = false;

        for (const auto& roomC : rooms) {
            if ((roomC.x == roomA.x && roomC.y == roomA.y) || (roomC.x == roomB.x && roomC.y == roomB.y)) {
                continue; // Пропускаем исходную и конечную комнаты
            }

            // Находим центр промежуточной комнаты
            int centerCX = roomC.x + roomC.width / 2;
            int centerCY = roomC.y + roomC.height / 2;

            adjustCorridorStartEnd(centerCX, centerCY, roomC);

            // Проверяем пересечение коридора с промежуточной комнатой
            if ((std::min(centerAX, centerBX) < centerCX && centerCX < std::max(centerAX, centerBX)) ||
                (std::min(centerAY, centerBY) < centerCY && centerCY < std::max(centerAY, centerBY))) {
                
                logger.log("Corridor intersects with room at (" + std::to_string(centerCX) + ", " + std::to_string(centerCY) + "). Creating corridors to bypass.");

                // Создаем коридоры от A до C и от C до B
                createCorridor(centerAX, centerAY, centerCX, centerCY);
                createCorridor(centerCX, centerCY, centerBX, centerBY);
                corridorConnected = true;
                break;
            }
        }

        if (!corridorConnected) {
            // Если коридор не пересекает другие комнаты, соединяем A и B напрямую
            createCorridor(centerAX, centerAY, centerBX, centerBY);
        }
    }
}

void LevelGenerator::placeStartAndEnd() {
    std::random_device rd;
    std::mt19937 g(rd());

    bool startPlaced = false;
    bool endPlaced = false;

    std::shuffle(rooms.begin(), rooms.end(), g);

    for (size_t i = 0; i < rooms.size(); ++i) {
        if (!startPlaced && rand() % 2 == 0) {
            int sx = rooms[i].x + rooms[i].width / 2;
            int sy = rooms[i].y + rooms[i].height / 2;
            dungeon[sy][sx] = 'S';
            logger.log("    Placed start point in room " + std::to_string(i) + " at (" + std::to_string(sx) + ", " + std::to_string(sy) + ")");
            startPlaced = true;
        } else if (!endPlaced && rand() % 2 == 0) {
            int ex = rooms[i].x + rooms[i].width / 2;
            int ey = rooms[i].y + rooms[i].height / 2;
            dungeon[ey][ex] = 'E';
            logger.log("    Placed end point in room " + std::to_string(i) + " at (" + std::to_string(ex) + ", " + std::to_string(ey) + ")");
            endPlaced = true;
        }

        if (startPlaced && endPlaced) {
            break;
        }
    }

    if (!startPlaced) {
        int sx = rooms.front().x + rooms.front().width / 2;
        int sy = rooms.front().y + rooms.front().height / 2;
        if (dungeon[sy][sx] != 'E') {  // Проверяем, что на этом месте нет конца уровня
            dungeon[sy][sx] = 'S';
            logger.log("Placed start point in first room at (" + std::to_string(sx) + ", " + std::to_string(sy) + ")");
        } else {
            for (const auto& room : rooms) {
                sx = room.x + room.width / 2;
                sy = room.y + room.height / 2;
                if (dungeon[sy][sx] == ' ') {
                    dungeon[sy][sx] = 'S';
                    logger.log("Placed start point in room at (" + std::to_string(sx) + ", " + std::to_string(sy) + ")");
                    break;
                }
            }
        }
    }

    if (!endPlaced) {
        int ex = rooms.back().x + rooms.back().width / 2;
        int ey = rooms.back().y + rooms.back().height / 2;
        if (dungeon[ey][ex] != 'S') {  // Проверяем, что на этом месте нет начала уровня
            dungeon[ey][ex] = 'E';
            logger.log("Placed end point in last room at (" + std::to_string(ex) + ", " + std::to_string(ey) + ")");
        } else {
            for (const auto& room : rooms) {
                ex = room.x + room.width / 2;
                ey = room.y + room.height / 2;
                if (dungeon[ey][ex] == ' ') {
                    dungeon[ey][ex] = 'E';
                    logger.log("Placed end point in room at (" + std::to_string(ex) + ", " + std::to_string(ey) + ")");
                    break;
                }
            }
        }
    }
}

void LevelGenerator::placeDoorsAfterCorridors() {
    logger.log("Placing doors after corridors...");
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            if (dungeon[y][x] == '#') {
                int spaceCount = 0;
                if (dungeon[y][x - 1] == ' ') spaceCount++;
                if (dungeon[y][x + 1] == ' ') spaceCount++;
                if (dungeon[y - 1][x] == ' ') spaceCount++;
                if (dungeon[y + 1][x] == ' ') spaceCount++;

                if (spaceCount >= 2) {
                    dungeon[y][x] = '|';
                    logger.log("Placed door at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                }
            }
        }
    }

    // Дополнительная проверка для превращения решеток в двери, если вокруг них 2 или более дверей
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            if (dungeon[y][x] == '#') {
                int doorCount = 0;
                if (dungeon[y][x - 1] == '|') doorCount++;
                if (dungeon[y][x + 1] == '|') doorCount++;
                if (dungeon[y - 1][x] == '|') doorCount++;
                if (dungeon[y + 1][x] == '|') doorCount++;

                if (doorCount >= 2) {
                    dungeon[y][x] = '|';
                    logger.log("Converted wall to door at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                }
            }
        }
    }
}

bool LevelGenerator::genRoomLevel() {
    int minSize = 50;
    int maxSize = 150;
    int width = rand() % (maxSize - minSize + 1) + minSize;
    int height = rand() % (maxSize - minSize + 1) + minSize;

    initialize(dungeon, width, height);
    WIDTH = width;
    HEIGHT = height;

    bool levelGenerated = false;
    int maxLevelAttempts = 50; // Ограничение на количество попыток генерации уровня

    loadRoomTemplates("levels/levelsGenerator/templates/roomTemplates", roomTemplates); // Загружаем шаблоны комнат

    int attempts = 0;
    while (!levelGenerated && attempts < maxLevelAttempts) {
        clearDungeon(dungeon, WIDTH, HEIGHT); // Очищаем карту перед генерацией
        generateRooms();

        connectRooms(); // Соединяем комнаты после их генерации

        placeDoorsAfterCorridors(); // Размещаем двери после создания коридоров

        placeStartAndEnd(); // Размещаем точки S и E после соединения комнаt
        
        saveToFile(dungeon, width, height, "levels/levelsGenerator/generatedLevels/level"); // Сохраняем уровень в файл
        
        levelGenerated = true;
        logger.log("Level generated successfully.");

        attempts++;
        if (levelGenerated) {
            break; // Прерываем цикл, если уровень сгенерирован успешно
        }
    }

    // Удаление динамически выделенной памяти
    if (dungeon != nullptr) {
        for (int i = 0; i < HEIGHT; i++) {
            delete[] dungeon[i];
        }
        delete[] dungeon;
        dungeon = nullptr;
    }

    if (!levelGenerated) {
        logger.log("Level generation failed after " + std::to_string(attempts) + " attempts.");
        return false; // если false, то будет генерироваться заново
    } else {
        return true; // не будет генерироваться заново
    }
}





