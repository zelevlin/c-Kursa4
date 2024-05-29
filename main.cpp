#include <ncurses.h>
#include <chrono>
#include <thread>
#include <iostream>
#include "player/character.h"
#include "player/controls.h"
#include "levels/levelManagement/level.h"
#include "levels/levelManagement/levelManager.h"
#include "player/inputState.h"
#include "enemies/enemy.h"
#include "screenManager.h"
#include "game/interface.h"
#include "inventory/InventoryIncludes.h"
#include "game/gameLoop.h"




InputState inputState;  // Глобальное определение состояния ввода (нужно)


// Функция для инициализации ncurses
void init() {
    initscr();          // Инициализация экрана
    cbreak();           // Линейный ввод
    noecho();           // Отключить эхо ввода
    curs_set(0);
    keypad(stdscr, TRUE);  // Разрешить клавиатурный ввод
    nodelay(stdscr, TRUE);    // Считывание getch не блокируется
    //halfdelay(1); // Установка тайм-аута для getch (обновление каждые 1/10 секунды)

}

//bool debugModAll = true;

int main() {
    init(); // Инициализация и настройка ncurses
    srand(time(0));
    // Очистка папки логов в начале игры
    Logger::clearLogDirectory("logs");

    Logger logger("game.log");  // Глобальное определение логгера
    //logger.checkAndClearLogFile();  // Проверка и очистка файла логгирования при необходимости
    bool menuRunning = true;
    while (menuRunning) {
        displayMainMenu();
        int ch = getch(); // Получение ввода от пользователя
        //ch = 3;
        switch (ch) {
            case '2': // Выход
                menuRunning = false;
                break;
            case '1': // infinity mode
                try {
                    //LevelManager::resetLevelNumber();
                    LevelManager::generateNewLevel();
                    LevelManager::loadLevel("levels/levelsGenerator/generatedLevels"); // Загружаем уровень
                    gameLoop(1); // Запускаем игровой цикл
                } catch (const std::exception& e) {
                    logger.log("An error occurred: " + std::string(e.what()));
                    return 1;
                }
                break;
            default:
                break;
        }
    }

    endwin(); // Завершение работы с ncurses
    return 0;
}
