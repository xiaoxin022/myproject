//
// Created by Administrator on 2022/10/25.
//
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>

using namespace std;

#pragma once

class Memory {
public:
    inline DWORD readDWORD(HANDLE hProcess, DWORD lpBaseAddress){
        DWORD lpBuffer;
        BOOL ret = ReadProcessMemory(hProcess, (LPCVOID)lpBaseAddress, &lpBuffer, 4, NULL);
        return lpBuffer;
    };
    inline DWORD readDWORD(HANDLE hProcess, HMODULE lpBaseAddress){
        DWORD lpBuffer;
        BOOL ret = ReadProcessMemory(hProcess, (LPCVOID)lpBaseAddress, &lpBuffer, 4, NULL);
        return lpBuffer;
    };

    inline BOOL write(HANDLE hProcess, LPVOID lpBaseAddress, DWORD lpBuffer){
        return WriteProcessMemory(hProcess, lpBaseAddress, (LPCVOID)lpBuffer, sizeof(lpBuffer), NULL);
    };
};

class Client {
public:
    DWORD dProcessId;
    HWND hWnd;
public:
    void getProcess(char processName[]) {
        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        PROCESSENTRY32 processentry32 = {sizeof(processentry32)};
        Process32First(handle, &processentry32);
        do {
            if (strcmp(processName, processentry32.szExeFile) == 0) {
                cout << "ProcessName=" << processentry32.szExeFile << endl;
                cout << "ProcessId=" << processentry32.th32ProcessID << endl;
                dProcessId = processentry32.th32ProcessID;
            }
        } while (Process32Next(handle, &processentry32));
    }
    DWORD getModuleArr(char moduleName[]){
        HMODULE hModule;
        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dProcessId);
        MODULEENTRY32 moduleentry32 = {sizeof(moduleentry32)};
        Module32First(handle,&moduleentry32);
        do {
            if (strcmp(moduleName, moduleentry32.szModule) == 0) {
                cout << "ModuleName=" << moduleentry32.szModule << endl;
                cout << "ModuleArr=" << moduleentry32.hModule << endl;
                hModule = moduleentry32.hModule;
            }
        } while (Module32Next(handle, &moduleentry32));
        return (DWORD)hModule;
    }
};