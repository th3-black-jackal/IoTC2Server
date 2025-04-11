#include "ConfigManager.h"
#include <algorithm>
#include <stdexcept>

ConfigManager& ConfigManager::Instance() {
    static ConfigManager instance;
    return instance;
}

void ConfigManager::Load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Failed to open config file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos) continue;

        std::string key = Trim(line.substr(0, equalPos));
        std::string value = Trim(line.substr(equalPos + 1));

        configMap[key] = value;
    }
}

bool ConfigManager::HasKey(const std::string& key) const {
    return configMap.find(key) != configMap.end();
}

std::string ConfigManager::Trim(const std::string& str) const {
    const char* whitespace = " \t\n\r";
    auto start = str.find_first_not_of(whitespace);
    auto end = str.find_last_not_of(whitespace);
    return start == std::string::npos ? "" : str.substr(start, end - start + 1);
}

template<typename T>
T ConfigManager::Get(const std::string& key) const {
    if (!HasKey(key)) {
        throw std::runtime_error("Config key not found: " + key);
    }

    std::istringstream iss(configMap.at(key));
    T value;
    if (!(iss >> value)) {
        throw std::runtime_error("Failed to convert value for key: " + key);
    }

    return value;
}

// Explicit template instantiations
template int ConfigManager::Get<int>(const std::string& key) const;
template double ConfigManager::Get<double>(const std::string& key) const;
template bool ConfigManager::Get<bool>(const std::string& key) const;
template std::string ConfigManager::Get<std::string>(const std::string& key) const;
