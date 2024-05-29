#include "screenManager.h"

std::vector<std::string> savedLevel; // Определение уровня
std::pair<int, int> cameraPosition;  // Позиция камеры (верхний левый угол)

void saveScreen(const std::vector<std::string>& levelMap) {
    savedLevel = levelMap;
    // Инициализируем начальную позицию камеры
    cameraPosition = {0, 0};
}

std::pair<int, int> updateCamera(int playerX, int playerY, int screenWidth, int screenHeight) {
    int halfScreenHeight = screenHeight / 2;
    int halfScreenWidth = screenWidth / 2;

    int drawStartX = playerX - halfScreenWidth;
    int drawStartY = playerY - halfScreenHeight;

    // Ограничение видимой области границами уровня
    if (drawStartX < 0) drawStartX = 0;
    if (drawStartY < 0) drawStartY = 0;
    if (drawStartX + screenWidth > savedLevel[0].size()) drawStartX = savedLevel[0].size() - screenWidth;
    if (drawStartY + screenHeight > savedLevel.size()) drawStartY = savedLevel.size() - screenHeight;

    // Убедимся, что drawStartX и drawStartY не отрицательны
    if (drawStartX < 0) drawStartX = 0;
    if (drawStartY < 0) drawStartY = 0;

    return {drawStartX, drawStartY};
}

void drawLevel(int drawStartX, int drawStartY, int screenWidth, int screenHeight) {
    clear(); // Очистка экрана перед отрисовкой

    for (int y = 0; y < screenHeight && (drawStartY + y) < savedLevel.size(); y++) {
        for (int x = 0; x < screenWidth && (drawStartX + x) < savedLevel[drawStartY + y].size(); x++) {
            mvaddch(y, x, savedLevel[drawStartY + y][drawStartX + x]);
        }
    }
}

void restoreScreen(Character& player, const std::vector<std::string>& levelMap) {
    int screenHeight, screenWidth;
    getmaxyx(stdscr, screenHeight, screenWidth);

    // Проверка необходимости обновления камеры
    std::pair<int, int> newCameraPosition = updateCamera(player.x, player.y, screenWidth, screenHeight);
    if (newCameraPosition != cameraPosition) {
        cameraPosition = newCameraPosition;
    }
    // Обновление смещения игрока
    player.offsetX = cameraPosition.first;
    player.offsetY = cameraPosition.second;

    drawLevel(cameraPosition.first, cameraPosition.second, screenWidth, screenHeight);
    refresh(); // Обновление экрана
}