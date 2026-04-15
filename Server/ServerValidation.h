#pragma once
#include <string>

// Header for server-side validation logic
namespace ServerValidation {
    bool ValidateHeartbeat(const std::string& clientId, const std::string& hash);
}
