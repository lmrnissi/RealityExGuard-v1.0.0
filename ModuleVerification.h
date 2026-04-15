#pragma once
#include <vector>
#include <string>

// List of required game modules for integrity verification
const std::vector<std::string> RequiredModules = {
    "GameCore.dll",
    "Engine.dll",
    "Renderer.dll",
    "Physics.dll"
};
