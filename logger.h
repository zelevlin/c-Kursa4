#pragma once
#include <fstream>
#include <iostream>
#include <string>

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();
    void log(const std::string& message);
    void checkAndClearLogFile();  // Метод для проверки и очистки файла
    static void clearLogDirectory(const std::string& directoryPath); // Новый метод для очистки папки логов

private:
    std::ofstream logFile;
    std::string filename;  // Имя файла для логгирования
};
