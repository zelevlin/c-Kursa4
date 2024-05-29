#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "../../logger.h"
#include "../levelManagement/level.h"

class LevelGenerator {
public:
    class Room {
    public:
        int x, y, width, height;
        Room(int x, int y, int width, int height) 
            : x(x), y(y), width(width), height(height) {}
    };

    static void generateRooms();
    static bool genRoomLevel();
    static void setRoomCountRange(int minRooms, int maxRooms);
    static void placeStartAndEnd();
    static void connectRooms();
    static void placeDoorsAfterCorridors(); // Добавляем метод для замены символов на двери
    

private:
    static char** dungeon;
    static int WIDTH;
    static int HEIGHT;
    static std::vector<Room> rooms;
    static std::vector<std::vector<std::string>> roomTemplates;

    static int getRandomRoomCount();
    static int minRooms;
    static int maxRooms;
    static Logger logger; // Статический логгер
};
