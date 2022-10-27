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
    inline BOOL readDWORD(HANDLE hProcess, DWORD lpBaseAddress) {
        DWORD lpBuffer;
        ReadProcessMemory(hProcess, (LPCVOID) lpBaseAddress, &lpBuffer, 4, NULL);
        return lpBuffer;
    };

    template<typename T>
    inline BOOL readFloat(HANDLE hProcess, DWORD lpBaseAddress, T lpBuffer) {
        return ReadProcessMemory(hProcess, (LPCVOID) lpBaseAddress, lpBuffer, 4, NULL);
    };

    template<typename T>
    inline BOOL readFloat(HANDLE hProcess, DWORD lpBaseAddress, T lpBuffer, SIZE_T size) {
        return ReadProcessMemory(hProcess, (LPCVOID) lpBaseAddress, lpBuffer, size, NULL);
    };


    inline BOOL write(HANDLE hProcess, LPVOID lpBaseAddress, DWORD lpBuffer) {
        return WriteProcessMemory(hProcess, lpBaseAddress, (LPCVOID) lpBuffer, sizeof(lpBuffer), NULL);
    };
};

class Paint {
public:
    HDC m_hdc;
    HWND hWnd;
    int m_HalfGameWidth;
    int m_HalfGameHigh;
    RECT m_GameRect;
public:
    Paint() {
        hWnd = FindWindow(NULL, "Counter-Strike");
        // 获取屏幕客户端界面
        GetClientRect(hWnd, &m_GameRect);
        m_HalfGameWidth = (m_GameRect.right - m_GameRect.left) * 0.5;
        m_HalfGameHigh = (m_GameRect.bottom - m_GameRect.top) * 0.5;

        // 设置画笔颜色，画出来的线条就是这个颜色
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(127, 255, 0));
        HBRUSH hBrush = (HBRUSH) GetStockObject(NULL_BRUSH);
        m_hdc = GetDC(hWnd);
        // 设置字体颜色
        SetTextColor(m_hdc, RGB(127, 255, 0));
        // 去除背景底色
        SetBkMode(m_hdc, TRANSPARENT);
        SelectObject(m_hdc, hPen);
        SelectObject(m_hdc, hBrush);
    }
};

class Client {
public:
    DWORD dProcessId;
    DWORD base_cstrike_exe;
    DWORD base_mp_dll;

public:
    Client() {
        getProcess("cstrike.exe");
        base_cstrike_exe = getModuleArr("cstrike.exe");
        base_mp_dll = getModuleArr("mp.dll");
    }

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

    DWORD getModuleArr(char moduleName[]) {
        HMODULE hModule;
        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dProcessId);
        MODULEENTRY32 moduleentry32 = {sizeof(moduleentry32)};
        Module32First(handle, &moduleentry32);
        do {
            cout << moduleentry32.szModule << endl;
            if (strcmp(moduleName, moduleentry32.szModule) == 0) {
                cout << "ModuleName=" << moduleentry32.szModule << endl;
                cout << "ModuleArr=" << moduleentry32.hModule << endl;
                hModule = moduleentry32.hModule;
            }
        } while (Module32Next(handle, &moduleentry32));
        return (DWORD) hModule;
    }
};