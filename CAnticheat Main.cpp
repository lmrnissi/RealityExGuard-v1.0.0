#include "pch.h"
#include <windows.h>
#include <vector>
#include <string>
#include "server.h"
typedef LONG NTSTATUS;
typedef NTSTATUS(NTAPI* pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG);

#define TH32CS_SNAPPROCESS 0x00000002
struct CUSTOM_PROCESSENTRY32A {
    DWORD dwSize;
    DWORD cntUsage;
    DWORD th32ProcessID;
    ULONG_PTR th32DefaultHeapID;
    DWORD th32ModuleID;
    DWORD cntThreads;
    DWORD th32ParentProcessID;
    LONG pcPriClassBase;
    DWORD dwFlags;
    CHAR szExeFile[MAX_PATH];
};

typedef HANDLE(WINAPI* CreateToolhelp32Snapshot_t)(DWORD, DWORD);
typedef BOOL(WINAPI* Process32FirstA_t)(HANDLE, CUSTOM_PROCESSENTRY32A*);
typedef BOOL(WINAPI* Process32NextA_t)(HANDLE, CUSTOM_PROCESSENTRY32A*);


void SecurityLoop() {
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");

    auto _CreateSnapshot = (CreateToolhelp32Snapshot_t)GetProcAddress(kernel32, "CreateToolhelp32Snapshot");
    auto _P32First = (Process32FirstA_t)GetProcAddress(kernel32, "Process32FirstA");
    auto _P32Next = (Process32NextA_t)GetProcAddress(kernel32, "Process32NextA");
    auto pSetInfo = (pNtSetInformationThread)GetProcAddress(ntdll, "NtSetInformationThread");

    std::vector<std::string> blacklist = { "cheatengine", "x64dbg", "processhacker" };

    while (true) {
        // 1. Basic & Remote Debug Check
        BOOL isRemoteDebuggerPresent = FALSE;
        CheckRemoteDebuggerPresent(GetCurrentProcess(), &isRemoteDebuggerPresent);
        if (IsDebuggerPresent() || isRemoteDebuggerPresent) {
            SendACinfoToServer("DebuggerDetected", "ActiveDebugger");
            exit(0);
        }

        // 2. Anti-Attach (The most important part)
        // ThreadHideFromDebugger (0x11) ensures any attach attempt crashes the tool
        if (pSetInfo) {
            pSetInfo(GetCurrentThread(), 0x11, NULL, 0);
        }

        // 3. Process verification (Blacklist)
        HANDLE hSnap = _CreateSnapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnap != INVALID_HANDLE_VALUE) {
            CUSTOM_PROCESSENTRY32A pe = { sizeof(CUSTOM_PROCESSENTRY32A) };
            if (_P32First(hSnap, &pe)) {
                do {
                    std::string name = pe.szExeFile;
                    for (auto& c : name) c = tolower(c);
                    for (const auto& item : blacklist) {
                        if (name.find(item) != std::string::npos) {
                            SendACinfoToServer("ForbiddenTool", name);
                            exit(0);
                        }
                    }
                } while (_P32Next(hSnap, &pe));
            }
            CloseHandle(hSnap);
        }

        // 4. Advanced: Hardware Breakpoints Check (Anti-Stepping)
        CONTEXT ctx = { 0 };
        ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        if (GetThreadContext(GetCurrentThread(), &ctx)) {
            if (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0) {
                exit(0);
            }
        }

        Sleep(1500);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason, LPVOID lpReserved) {
    if (ul_reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SecurityLoop, NULL, 0, NULL);
        if (hThread) CloseHandle(hThread);
    }
    return TRUE;
}

// Note : this anticheat is just a example