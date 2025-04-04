#include "Logger.h"
#include <iostream>

Logger &Logger::Instance(){
    static Logger instance;
    return instance;
}
void Logger::Log(LogLevel level, const std::string &message){
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout<<GetColorCode(level)<<GetPrefix(level)<<message<<"\033[0m" <<std::endl;
}

std::string Logger::GetPrefix(LogLevel level){
    switch(level){
        case LogLevel::INFO: return "[*] ";
        case LogLevel::WARN: return "[!] ";
        case LogLevel::ERROR: return "[-] ";
        case LogLevel::RESP: return "[-->] ";
        default: return "";
    }
}
std::string Logger::GetColorCode(LogLevel level){
    switch(level){
        case LogLevel::INFO: return "\033[1;32m";   //Green
        case LogLevel::WARN: return "\033[1;33m";   //Yellow
        case LogLevel::ERROR: return "\033[1;31m";  //Red
        case LogLevel::RESP: return "\033[1;34m";   //Blue
        default: return "\033[0m";
    }
}