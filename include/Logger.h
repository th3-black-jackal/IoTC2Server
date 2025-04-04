#ifndef LOGGER_H
#define LOGGER_H


#include <string>
#include <mutex>

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
    private:
        Logger() = default;
        std::mutex logMutex;
        std::string GetPrefix(LogLevel level);
        std::string GetColorCode(LogLevel level);
};

#endif