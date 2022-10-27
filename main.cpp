#include <iostream>

#include <math.h>
#include <memory>
#include "process.h"

Client client;
Memory memory;

HANDLE hProcess;


struct Vec3 {
    float x, y, z;
};
struct Mat3x4 {
    float a[12];
};
struct Mat4x4 {
    float a[16];
};

class Player {
public:
    //位置x,y,z
    Vec3 location;
    //坐标矩阵
    Mat4x4 mat;
    float hp;
    int num;
public:
    Player(int num) {
        this->num = num;
    };

    Player() {};

    void getHp(HANDLE hProcess) {
        DWORD first = memory.readDWORD(hProcess, client.base_cstrike_exe + 0x11069BC);
        DWORD second = memory.readDWORD(hProcess, first + 0x7c);
        DWORD third = memory.readDWORD(hProcess, second + 0x4);
        memory.readFloat(hProcess, third + 0x160, &hp);
        cout << "blood=" << hp << endl;
    };

    void getLocation(HANDLE hProcess) {
        DWORD first = memory.readDWORD(hProcess, client.base_cstrike_exe + 0x11069BC);
        memory.readFloat(hProcess, first + num * 0x324 + 0x88, &location.x);
        memory.readFloat(hProcess, first + num * 0x324 + 0x8c, &location.y);
        cout << "x=" << location.x << endl;
        cout << "y=" << location.y << endl;
    }
};

int player_nums = 1;
Player players[20];

DWORD getPlayerNums(HANDLE hProcess) {
    player_nums = memory.readFloat(hProcess, client.base_mp_dll + 0x114960, &player_nums);
    return player_nums;
}

void init() {
    client.getProcess("cstrike.exe");
    client.base_cstrike_exe = client.getModuleArr("cstrike.exe");
    client.base_mp_dll = client.getModuleArr("mp.dll");
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, client.dProcessId);
}

int main() {
    init();
    for (int i = 0; i < 20; ++i) {
        Player player(i + 1);
        players[i] = player;
    }
    while (true) {
        for (int i = 0; i < player_nums; ++i) {
            players[i].getHp(hProcess);
            cout << "hp=" << players[i].hp << endl;
            players[i].getLocation(hProcess);
        }
        player_nums = getPlayerNums(hProcess);
    }
    return 0;
}