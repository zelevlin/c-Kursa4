// logger.cpp
#include "logger.h"
#include <fstream>
#include <string>


namespace fs = std::filesystem;

Logger::Logger(const std::string& filename) {
    // Определяем путь к папке logs
    fs::path logDir = "logs";

    // Создаем папку logs, если она не существует
    if (!fs::exists(logDir)) {
        fs::create_directory(logDir);
    }

    // Полный путь к файлу
    fs::path logFilePath = logDir / filename;

    // Открываем файл для записи, в режиме добавления
    logFile.open(logFilePath, std::ios::out | std::ios::app); 
    if (!logFile.is_open()) {
        std::cerr << "Ошибка при открытии файла для логгирования: " << logFilePath << std::endl;
    }
}

void Logger::checkAndClearLogFile() {
    std::ifstream inFile(filename);
    int lineCount = 0;
    std::string line;

    while (std::getline(inFile, line)) {
        ++lineCount;
    }

    if (lineCount > 200) {
        inFile.close();
        std::ofstream clearFile(filename, std::ios::out | std::ios::trunc);
        clearFile.close();
        std::cout << "Log file cleared due to excessive length." << std::endl;
    }
}

void Logger::log(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl; // Записываем сообщение в файл
    } else {
        std::cerr << "Не удается записать сообщение в лог, так как файл не открыт." << std::endl;
    }
}

void Logger::clearLogDirectory(const std::string& directoryPath) {
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (fs::is_regular_file(entry.path())) {
            // Открываем файл в режиме записи, что очищает его содержимое
            std::ofstream ofs(entry.path(), std::ios::out | std::ios::trunc);
            ofs.close();
        }
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close(); // Закрываем файл при уничтожении объекта
    }
}