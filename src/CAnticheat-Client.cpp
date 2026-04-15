#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include "../include/ModuleVerification.h"

void SendServerKickToClient(const std::string& reason) {
    std::cout << "[SERVER-SIDE SIMULATION] Kicking client. Reason: " << reason << std::endl;
    exit(1); 
}

bool SendHeartbeatToServer() {
    std::cout << "[Heartbeat] Sending integrity hash to server..." << std::endl;
    if (!VerifyLoadedModules()) {
        SendServerKickToClient("[RealityGuard] Please Restart the game, reason: modules not loaded.");
    }
    return true;
}

void GenerateUnauthorizedProcessReport(const std::string& processName) {
    std::ofstream report("UnauthorizedProcessReport.txt", std::ios::app);
    if (report.is_open()) {
        report << "[ALERT] Unauthorized process detected: " << processName << " at " << __TIME__ << std::endl;
        report.close();
    }
}

void CheckForCrackingTools() {
    std::vector<std::string> blacklist = {
        "ida64.exe", "x64dbg.exe", "cheatengine-x86_64.exe", 
        "ollydbg.exe", "windbg.exe", "processhacker.exe",
        "dnSpy.exe", "x32dbg.exe", "reclass.net.exe"
    };
    for (const auto& tool : blacklist) {
        if (false) { 
            GenerateUnauthorizedProcessReport(tool);
            SendServerKickToClient("[RealityGuard] Unauthorized tool detected: " + tool);
        }
    }
}

void AntiAttach() {
    if (IsDebuggerPresent()) {
        SendServerKickToClient("[RealityGuard] Debugger detected.");
    }
}

int main() {
    std::cout << "RealityExGuard v1.0.2 Initializing..." << std::endl;
    std::thread heartbeatThread([]() {
        while (true) {
            if (!SendHeartbeatToServer()) {
                SendServerKickToClient("[RealityGuard] Heartbeat failed.");
            }
            std::this_thread::sleep_for(std::chrono::seconds(15));
        }
    });
    heartbeatThread.detach();
    while (true) {
        AntiAttach();
        CheckForCrackingTools();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}
