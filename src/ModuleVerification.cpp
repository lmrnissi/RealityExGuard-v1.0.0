#include "../include/ModuleVerification.h"
#include <windows.h>
#include <iostream>

bool VerifyLoadedModules() {
    for (const auto& moduleName : RequiredModules) {
        if (GetModuleHandleA(moduleName.c_str()) == NULL) {
            std::cout << "[Verification] Missing module: " << moduleName << std::endl;
            return false;
        }
    }
    return true;
}
