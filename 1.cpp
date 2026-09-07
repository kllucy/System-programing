#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

int main() {
    
    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS,
        0);

    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Error creating snapshot\n"; 
        return 1;
    }


    PROCESSENTRY32 process;
    process.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &process)) {
        do {
            std::cout << "PID: " << process.th32ProcessID;
            std::cout << " | ";
            std::wcout << "Name: " << process.szExeFile << "\n";
        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);
    return 0;
}
