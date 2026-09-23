#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
using namespace std;

int main() {
    HANDLE snapshot = CreateToolhelp32Snapshot(
        // способ обратиться к объекту window
        // снимок системной информации
        // получаем из CreateToolhelp32Snapshot информаию - хранилище
        TH32CS_SNAPPROCESS,
        // хранит и показывает только поцесс, зависит от флага команды _...PROCESS
        0
    );
    // следует добавлять проверку
    if (snapshot == INVALID_HANDLE_VALUE) {
        // если не получается обратиться к объекту, то программа выдает ошибку
        cout << "Error\n";
        return 1;
    }
    PROCESSENTRY32 process;
    // PROCESSENTRY32 записывает информацию: логов, размеров, индификаторы процессов, количество и приоритеты потоков
    process.dwSize = sizeof(PROCESSENTRY32);
    // int processCont = 0;
    if (Process32First(snapshot, &process)) {
		// получает дескриптор, выходт в хранилище и берет первый процесс
        do {
            // cout << "PID: " << process.th32ProcessID;
            // cout << " | ";
            // wcout << "Name: " << process.szExeFile << '\n';
			// второй вариант записи информации
			// wcout << L"PID: " << process.th32ProcessID << L" | Name: " << process.szExeFile << '\n';
			DWORD pid = process.th32ProcessID
			HANDLE hProcess = OpenProcess(
				// OpenProcess принимает 3 парметра(передача дескриптора, настроки и адреса)
				PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
				false,
				pid
			);
			if (hProcess != NULL){
				PROCESS_MEMORY_COUNTERS memory;
				// создаем экземпляр структуры
				if (GetProcessMemoryInfo(hProcess, &memory, sizeof(memory))){
					wcout << pid << " " << process.szExeFile << " " << memory.WorkingSetSize / 1024 / 1024 << L"MB\n";
				}
			}
			CloseHandle(hProcess)
			// в ядре хранятся структры, EPROCESS - одна из них, объект, в котором будут хранится данные
	    	// processCont++;
        	} 
		while (Process32Next(snapshot, &process));
    }
    CloseHandle(snapshot);
    return 0;
}
