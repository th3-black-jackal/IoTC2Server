#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <type_traits>

class ConfigManager {
public:
    static ConfigManager& Instance();

    void Load(const std::string& filepath);

    bool HasKey(const std::string& key) const;

    template<typename T>
    T Get(const std::string& key) const;

private:
    ConfigManager() = default;
    std::unordered_map<std::string, std::string> configMap;

    std::string Trim(const std::string& str) const;
};
