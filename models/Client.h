#pragma once
#include <string>
#include <optional>

class Client {
public:
    Client() = default;
    Client(int id, const std::string& name, const std::string& deviceType,
           const std::string& ip, const std::string& regTime,
           const std::optional<std::string>& lastSeen,
           const std::optional<std::string>& metadata);

    // Getters
    int getId() const;
    const std::string& getName() const;
    const std::string& getDeviceType() const;
    const std::string& getIp() const;
    const std::string& getRegistrationTime() const;
    const std::optional<std::string>& getLastSeen() const;
    const std::optional<std::string>& getMetadata() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setDeviceType(const std::string& type);
    void setIp(const std::string& ip);
    void setRegistrationTime(const std::string& time);
    void setLastSeen(const std::optional<std::string>& time);
    void setMetadata(const std::optional<std::string>& data);

    static std::string tableName();

private:
    int clientId{};
    std::string clientName;
    std::string deviceType;
    std::string clientIp;
    std::string registrationTimestamp;
    std::optional<std::string> lastSeen;
    std::optional<std::string> metadata;
};
