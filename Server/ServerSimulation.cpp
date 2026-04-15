// Simulated server-side logic for heartbeat validation and kick handling
#include <iostream>
#include <string>

void HandleClientKick(const std::string& clientId, const std::string& reason) {
    std::cout << "[SERVER] Kicking Client: " << clientId << ". Reason: " << reason << std::endl;
}

void ProcessHeartbeat(const std::string& clientId, const std::string& hash) {
    std::cout << "[SERVER] Received heartbeat from: " << clientId << " | Hash: " << hash << std::endl;
}
