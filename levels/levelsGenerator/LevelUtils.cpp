#include "LevelUtils.h"
#include "LevelGenerator.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <filesystem>

void initialize(char**& dungeon, int width, int height) {
    dungeon = new char*[height];
    for (int i = 0; i < height; i++) {
        dungeon[i] = new char[width];
        for (int j = 0; j < width; j++) {
            dungeon[i][j] = '&';
        }
    }
}

void clearDungeon(char** dungeon, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            dungeon[i][j] = '&';
        }
    }
}

void saveToFile(char** dungeon, int width, int height, const char* baseFilename) {
    static int fileIndex = 1;
    std::string filename;

    std::ifstream testFile;
    do {
        std::ostringstream filenameStream;
        filenameStream << baseFilename << fileIndex << ".txt";
        filename = filenameStream.str();
        testFile.open(filename.c_str());
        if (testFile.is_open()) {
            testFile.close();
            fileIndex++;
        } else {
            break;
        }
    } while (true);

    std::ofstream file(filename.c_str());
    if (file.is_open()) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file << dungeon[y][x];
            }
            file << '\n';
        }
        file.close();
    }
}

double calculateDistance(int x1, int y1, int x2, int y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool intersects(const LevelGenerator::Room& a, const LevelGenerator::Room& b, int buffer) {
    return (a.x - buffer < b.x + b.width + buffer &&
            a.x + a.width + buffer > b.x - buffer &&
            a.y - buffer < b.y + b.height + buffer &&
            a.y + a.height + buffer > b.y - buffer);
}

bool findEmpty3x3(char** dungeon, int width, int height, int &x, int &y, int excludeX, int excludeY) {
    for (int j = 1; j < height - 2; j++) {
        for (int i = 1; i < width - 2; i++) {
            if ((excludeX != -1 && excludeY != -1 && std::abs(excludeX - (i + 1)) <= 4 && std::abs(excludeY - (j + 1)) <= 4)) {
                continue;
            }
            if (dungeon[j][i] == ' ' && dungeon[j][i+1] == ' ' && dungeon[j][i+2] == ' ' &&
                dungeon[j+1][i] == ' ' && dungeon[j+1][i+1] == ' ' && dungeon[j+1][i+2] == ' ' &&
                dungeon[j+2][i] == ' ' && dungeon[j+2][i+1] == ' ' && dungeon[j+2][i+2] == ' ') {
                x = i + 1;
                y = j + 1;
                return true;
            }
        }
    }
    return false;
}

void loadRoomTemplates(const std::string& templateDirectory, std::vector<std::vector<std::string>>& roomTemplates) {
    for (const auto& entry : std::filesystem::directory_iterator(templateDirectory)) {
        if (entry.path().extension() == ".txt") {
            std::ifstream file(entry.path());
            std::vector<std::string> templateLines;
            std::string line;
            while (std::getline(file, line)) {
                templateLines.push_back(line);
            }
            roomTemplates.push_back(templateLines);
        }
    }
}
