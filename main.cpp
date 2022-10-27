#include <iostream>

#include <math.h>
#include <memory>
#include "process.h"

Client client;
Paint paint;
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
    //世界坐标 x,y,z
    Vec3 world;
    //屏幕坐标 x,y,z
    Vec3 screen;
    float hp;
    int num;
    //阵营
    int flag;
public:
    Player(int num) {
        this->num = num;
        getFlag(hProcess);
    };

    Player() {};

    static float getDistance(Vec3 m, Vec3 p){
        return sqrt(powf(m.x - p.x, 2) + powf(m.y - p.y, 2) + powf(m.z - p.z, 2));
    };

    void getHp(HANDLE hProcess) {
        DWORD first = memory.readDWORD(hProcess, client.base_cstrike_exe + 0x11069BC);
        DWORD second = memory.readDWORD(hProcess, first + num * 0x324 + + 0x7c);
        DWORD third = memory.readDWORD(hProcess, second + 0x4);
        memory.readFloat(hProcess, third + 0x160, &hp);
    };

    void getLocation(HANDLE hProcess) {
        DWORD first = memory.readDWORD(hProcess, client.base_cstrike_exe + 0x11069BC);
        memory.readFloat(hProcess, first + num * 0x324 + 0x88, &world.x);
        memory.readFloat(hProcess, first + num * 0x324 + 0x8c, &world.y);
        memory.readFloat(hProcess, first + num * 0x324 + 0x90, &world.z);
    }

    void getFlag(HANDLE hProcess) {
        DWORD first = memory.readDWORD(hProcess, client.base_cstrike_exe + 0x11069BC);
        DWORD second =memory.readDWORD(hProcess, first + num * 0x324 + 0x7c);
        memory.readFloat(hProcess, second + 0x1c8, &flag);
    }
};

int player_nums = 3;
Player players[20];
Mat4x4 mat;

DWORD getPlayerNums(HANDLE hProcess) {
//    player_nums =  memory.readFloat(hProcess, client.base_mp_dll + 0x114960, &player_nums);
return player_nums;
}

void updateMat() {
    memory.readFloat(hProcess, client.base_cstrike_exe + 0x1820100, &mat.a, sizeof(mat.a));
}

void init() {
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, client.dProcessId);
}

int main() {
    init();
    for (int i = 0; i < 20; ++i) {
        Player player(i);
        players[i] = player;
    }

    while (true) {
        for (int i = 1; i <= player_nums; ++i) {
            if (players[i].flag == players[0].flag){
                continue;
            }
            players[i].getHp(hProcess);
            if (players[i].hp <= 0){
                continue;
            }
            updateMat();

            //cout << "hp=" << players[i].hp << endl;
            players[i].getLocation(hProcess);
            players[i].screen.z =
                    mat.a[2] * players[i].world.x + mat.a[6] * players[i].world.y + mat.a[10] * players[i].world.z +
                    mat.a[14];
            if (players[i].screen.z < 0.001f) {
                continue;
            }
            players[i].screen.x = paint.m_HalfGameWidth +
                                  (mat.a[0] * players[i].world.x + mat.a[4] * players[i].world.y +
                                   mat.a[8] * players[i].world.z + mat.a[12]) / players[i].screen.z *
                                  paint.m_HalfGameWidth;
            players[i].screen.y = paint.m_HalfGameHigh -
                                  (mat.a[1] * players[i].world.x + mat.a[5] * players[i].world.y +
                                   mat.a[9] * players[i].world.z + mat.a[13]) / players[i].screen.z *
                                  paint.m_HalfGameHigh;
            RECT rect;
            rect.left = players[i].screen.x;
            rect.top = players[i].screen.y;
            rect.right = players[i].screen.x + 100;
            rect.bottom = players[i].screen.y + 40;
            LPRECT lprc = (struct tagRECT *) malloc(sizeof(struct tagRECT));
            lprc->left = rect.left;
            lprc->right = rect.left + 100;
            lprc->top = rect.top;
            lprc->bottom = rect.top + 40;
            float distance = Player::getDistance(players[0].world, players[i].world);
            DrawText(paint.m_hdc, distance, -1, lprc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            Rectangle(paint.m_hdc, rect.left, rect.top, rect.right, rect.bottom);
            cout << "x=" << players[i].screen.x << ",y=" << players[i].screen.y << endl;
        }
        player_nums = getPlayerNums(hProcess);
    }
    return 0;
}