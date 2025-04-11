#pragma once
#ifndef LOGGER_H
#define LOGGER_H


#include <string>
#include <mutex>
#include <ostream>
#include <atomic>
#include <iostream>

enum class LogLevel{
    INFO,
    WARN,
    ERROR,
    RESP
};

class Logger{
    public:
        static Logger& Instance();
        void Log(LogLevel level, const std::string &message);
        void setOutputStream(std::ostream& stream);
        void enableColor(bool enable);
    private:
        Logger() = default;
        Logger(const Logger&) = delete;
        Logger &operator=(const Logger&) = delete;
        std::mutex logMutex;
        std::ostream *outputStream = &std::cout;
        std::string GetPrefix(LogLevel level);
        std::string GetColorCode(LogLevel level);
        std::atomic<bool> colorEnabled {true};
      
};

#endif