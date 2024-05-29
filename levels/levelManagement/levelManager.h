#pragma once
#include "level.h"  // Включаем существующее определение Level
#include <string>
#include "../levelsGenerator/LevelGenerator.h"
#include "../../logger.h"
//#include <vector>

class LevelManager {
public:
    // Загружает уровень из файла
    static bool loadLevel(const std::string& filename, Level& level);
    // Отображает уровень
    static void displayLevel(const Level& level);
    // Добавляет уровень в список уровней
    static void addLevel(const Level& level);
    // Получает уровень по индексу
    static Level& getLevel(size_t index);
    // Удаляет уровень по индексу
    static void removeLevel(size_t index);
    // Загружает и перемешивает уровни
    static void loadAndShuffleLevels(std::string path);
    // генерирует новый уровень
    static void generateNewLevel();
    //загружает уровень
    static void loadLevel(std::string path);
    //static void loadLevel(std::string path, int levelNumber);
    static bool findPoints(Level& level);
    //static void resetLevelNumber();

    static size_t countLevels();

    static void generate10levels();

    static void clearDirectory(const std::string& directoryPath);
    
private:
    static std::vector<Level> levels; // Статический список, хранящий уровни
    static Logger logger;
    //static int currentLevelNumber; // Переменная для хранения текущего номера уровня
};




