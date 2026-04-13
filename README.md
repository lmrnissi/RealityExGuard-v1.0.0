# RealityExGuard v1.0.0 🛡️

A lightweight, educational C++ Anti-Cheat DLL designed to demonstrate core concepts of game security, including anti-debugging, anti-attachment, and server-side reporting simulations.

## 📌 Project Purpose
This project is an **Open Source Educational Example**. It is designed for developers and cybersecurity researchers to understand how low-level Windows APIs can be used to protect a process from common manipulation tools like Cheat Engine, x64dbg, and Scylla.

> **Note:** This is a demonstration of concept. In a production environment, these techniques should be combined with kernel-mode drivers and robust server-side validation.

## ✨ Features

### 1. Anti-Attachment (The "Cheat Engine Killer")
Uses `NtSetInformationThread` with the `ThreadHideFromDebugger` (0x11) flag. This ensures that if a debugger or tool like Cheat Engine attempts to attach to the guarded thread, the process will either crash or cause the tool to hang.

### 2. Advanced Anti-Debugging
- **Hardware Breakpoint Detection:** Scans CPU Debug Registers (`Dr0`-`Dr3`) to detect "stepping" or instruction-level monitoring.
- **Timing Checks:** Utilizes `__rdtsc` to measure execution time. If the time delta is too high, it assumes a debugger has paused the process.
- **Remote Debugger Check:** Uses `CheckRemoteDebuggerPresent` to identify external debugger engines.

### 3. Process Blacklisting
A dynamic scan of the system's process environment to identify forbidden tools (e.g., `Process Hacker`, `x64dbg`) using a custom implementation of the Toolhelp32 API for better compatibility.

### 4. Server-Side Simulation
Includes a `SendACinfoToServer` function template. This demonstrates how a client-side detection should be packaged and sent to a backend (like Firebase or a custom API) for logging and HWID banning.

## 🚀 Technical Implementation
The project is written in **Native C++** and uses dynamic linking (`GetProcAddress`) for critical functions to avoid static detection of imports.

### Build Instructions
1. Open `RealityExGuard 1.0.0.slnx` in **Visual Studio 2026**.
2. Set the platform to **Release | x86**.
3. Ensure the Character Set is set to **Multi-Byte**.
4. Build the solution to generate `RealityExGuard.dll`.

## 🤝 Collaboration
This project was developed by **Lmrnissi(Reality Software)**. 
Special thanks to the community for security insights.

---
*Disclaimer: This software is for educational purposes only. Do not use it to violate the Terms of Service of any software or game.*
