#include "Client.h"

// Constructor
Client::Client(int id, const std::string& name, const std::string& deviceType,
               const std::string& ip, const std::string& regTime,
               const std::optional<std::string>& lastSeen,
               const std::optional<std::string>& metadata)
    : clientId(id), clientName(name), deviceType(deviceType), clientIp(ip),
      registrationTimestamp(regTime), lastSeen(lastSeen), metadata(metadata) {}

// Getters
int Client::getId() const { return clientId; }
const std::string& Client::getName() const { return clientName; }
const std::string& Client::getDeviceType() const { return deviceType; }
const std::string& Client::getIp() const { return clientIp; }
const std::string& Client::getRegistrationTime() const { return registrationTimestamp; }
const std::optional<std::string>& Client::getLastSeen() const { return lastSeen; }
const std::optional<std::string>& Client::getMetadata() const { return metadata; }

// Setters
void Client::setId(int id) { clientId = id; }
void Client::setName(const std::string& name) { clientName = name; }
void Client::setDeviceType(const std::string& type) { deviceType = type; }
void Client::setIp(const std::string& ip) { clientIp = ip; }
void Client::setRegistrationTime(const std::string& time) { registrationTimestamp = time; }
void Client::setLastSeen(const std::optional<std::string>& time) { lastSeen = time; }
void Client::setMetadata(const std::optional<std::string>& data) { metadata = data; }

// Table name
std::string Client::tableName() {
    return "clients";
}
