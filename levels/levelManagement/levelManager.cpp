#include "levelManager.h"
#include "level.h"
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <filesystem>
#include <random>
//int LevelManager::currentLevelNumber = 0; // Определение статической переменной

namespace fs = std::filesystem;
std::vector<Level> LevelManager::levels;
Logger LevelManager::logger("levelManager.log"); // Инициализация логгера

// void LevelManager::resetLevelNumber(){
//     LevelManager::currentLevelNumber = 0;
// }

bool LevelManager::loadLevel(const std::string& filename, Level& level) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        logger.log("Failed to open file: " + filename);
        return false;
    }

    level.map.clear(); // Очистка карты перед загрузкой нового уровня
    std::string line;
    int y = 0;
    int maxWidth = 0;

    while (getline(file, line)) {
        if (line.length() > maxWidth) {
            maxWidth = line.length();
        }
        level.map.push_back(line);

        // Поиск символа 'E'
        int endX = line.find('E');
        if (endX != std::string::npos) {
            level.endX = endX-1;
            level.endY = y-1;
        }

        // Поиск символа 'S'
        int startX = line.find('S');
        if (startX != std::string::npos) {
            level.startX = startX-1;
            level.startY = y-1;
        }

        y++;
    }

    level.width = maxWidth;
    level.height = y;
    
    file.close();
    return true;
}
void LevelManager::addLevel(const Level& level) {
    levels.push_back(level);
}

Level& LevelManager::getLevel(size_t index) {
    if (index < levels.size()) {
        return levels[index];
    } else {
        throw std::out_of_range("Index out of range");
    }
}

void LevelManager::removeLevel(size_t index) {
    if (index < levels.size()) {
        levels.erase(levels.begin() + index);
    } else {
        throw std::out_of_range("Index out of range");
    }
}

void LevelManager::displayLevel(const Level& level) {
    clear(); // Очистка экрана
    for (const auto& line : level.map) {
        printw("%s\n", line.c_str());
    }
    refresh(); // Обновление экрана
}

void LevelManager::loadAndShuffleLevels(std::string path) {
    //std::string path = "levels/levelData";
    std::vector<std::string> levelFiles;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".txt") {
            levelFiles.push_back(entry.path().string());
        }
    }

    // std::random_device rd;
    // std::mt19937 g(rd());
    // std::shuffle(levelFiles.begin(), levelFiles.end(), g);

    for (const auto& filename : levelFiles) {
        Level level;
        if (loadLevel(filename, level)) {
            addLevel(level);
        }
    }
}

size_t LevelManager::countLevels() {
    return levels.size();
}

void LevelManager::generate10levels(){
   
    clearDirectory("levels/levelsGenerator/generatedLevels");
    //endwin();
    for (size_t i = 0; i < 10; i++)
    {
        if (!LevelGenerator::genRoomLevel()) i--;
    }
    
}

void LevelManager::generateNewLevel(){
    LevelGenerator::genRoomLevel();
    //currentLevelNumber++;
}

bool LevelManager::findPoints(Level& level) {
    bool foundStart = false;
    bool foundEnd = false;

    for (int y = 0; y < level.map.size(); y++) {
        const std::string& line = level.map[y];
        int startX = line.find('S');
        if (startX != std::string::npos) {
            level.startX = startX;
            level.startY = y;
            foundStart = true;
        }

        int endX = line.find('E');
        if (endX != std::string::npos) {
            level.endX = endX;
            level.endY = y;
            foundEnd = true;
        }

        // Если обе точки найдены, прекращаем дальнейший поиск
        if (foundStart && foundEnd) {
            break;
        }
    }
    if (!(foundStart && foundEnd)){
        logger.log("Failed to find start or end point in level");
    } else{
        logger.log("find start at "+std::to_string(level.startX)+", " + std::to_string(level.startY)+ " and end point in level successfully at "+ std::to_string(level.endX)+", " + std::to_string(level.endY) );
        //logger.log("Character spawned at (" + std::to_string(x) + ", " + std::to_string(y) + ")");

    }
    return foundStart && foundEnd;
}

void LevelManager::loadLevel(std::string path) {
    std::vector<std::string> levelFiles;

    

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".txt") {
            levelFiles.push_back(entry.path().string());
        }
    }
    for (const auto& filename : levelFiles) {
        Level level;
        //level.number = currentLevelNumber;
        //const std::string& filename = levelFiles.back();
        if (loadLevel(filename, level)) {
            addLevel(level);
            LevelManager::findPoints(level);
        }
    }
}

 // Очистка папки перед началом генерации уровней
void LevelManager::clearDirectory(const std::string& directoryPath) {
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        std::filesystem::remove_all(entry.path());
    }
}

