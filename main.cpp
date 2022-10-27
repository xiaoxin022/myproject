#include <iostream>

#include <math.h>
#include "process.h"

Client client;
Memory memory;

int main() {
    client.getProcess("cstrike.exe");
    DWORD base = client.getModuleArr("cstrike.exe");
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS,false,client.dProcessId);
    DWORD first = memory.readDWORD(handle,base + 0x11069BC);
    DWORD second = memory.readDWORD(handle,first + 0x7c);
    DWORD third = memory.readDWORD(handle,second + 0x4);
    DWORD fouth = memory.readDWORD(handle,third + 0x160);
    cout << "blood=" <<  float(fouth) << endl;
    return 0;
}