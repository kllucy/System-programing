#include <iostream>
#include <string>
#include <windows.h>

int main()
{
    while (true) {

        setlocale(LC_ALL, "RU");
        char path[MAX_PATH];

        GetCurrentDirectoryA(MAX_PATH, path);
        std::cout << path;
        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) {
            continue;
        }
        if (input == "exit") {
            std::cout << "Выход из myshell. До свидания!" << std::endl;
            break;
        }
        else if (input == "help") {
            std::cout << " Доступные команды myshell" << std::endl;
            std::cout << "  help           - Показать эту справку" << std::endl;
            std::cout << "  pwd            - Показать текущую рабочую директорию" << std::endl;
            std::cout << "  cd [путь]      - Сменить рабочую директорию" << std::endl;
            std::cout << "  exit           - Выйти из оболочки" << std::endl;
            std::cout << "  [любая другая] - Запуск внешней программы Windows (например: calc, notepad, ping)\n" << std::endl;
            continue;
        }
        else if (input == "pwd") {
            std::cout << path << std::endl;
            continue;
        }
        else if (input.rfind("cd ", 0) == 0) {
            std::string new_path = input.substr(3);

            if (!SetCurrentDirectoryA(new_path.c_str())) {
                std::cout << "Система не менеджер найти указанный путь: " << new_path << std::endl;
            }
            continue;

        }
    }
}
Создание собственной командной оболочки (Mini-Shell)

Критерии
* Программа работает в бесконечном цикле, выводя приглашение к вводу (например, `myshell> `).
* Корректно работают встроенные команды `help`, `pwd` и `exit`.
* Любая внешняя команда (например, `ping`, `ls`, `mkdir` или `calc`) запускается как *дочерний процесс*.
* Родительский процесс (ваш Shell) терпеливо ждет, пока дочерний процесс завершит работу, и только потом выводит новое приглашение к вводу.
* Программа не «падает» и не завершает работу аварийно при вводе несуществующей команды.


Документация в помощь:

Вариант для Linux (POSIX API)
1. `fork()` — распараллеливает программу на два идентичных процесса. Возвращает `0` внутри дочернего процесса и `PID` дочернего процесса внутри родительского.
2. `execvp(const char *file, char *const argv[])` — заменяет текущий образ процесса новым процессом. Первым аргументом принимает имя программы, вторым — массив строк-аргументов (последний элемент `NULL`).
3. `waitpid(pid_t pid, int *status, int options)` — заставляет родительский процесс заснуть и ждать, пока дочерний процесс не завершит работу.
4. `getcwd(char *buf, size_t size)` — системный вызов для получения пути к текущей рабочей директории.

Логическая шпаргалка для Linux:

pid_t pid = fork();
if (pid == 0) {
    execvp(args[0], args);
    perror("Ошибка запуска");
    exit(1);
} else if (pid > 0) {
    waitpid(pid, &status, 0);
}

Вариант для Windows (WinAPI)
1. `CreateProcessA(...)` — создает новый процесс и его главный поток.
2. `WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds)` — приостанавливает поток, пока указанный объект (hProcess) не завершится. Используйте `INFINITE`.
3. `CloseHandle(HANDLE hObject)` — закрывает открытые дескрипторы процесса и потока.
4. `GetCurrentDirectoryA(DWORD nBufferLength, LPSTR lpBuffer)` — функция WinAPI для получения текущего пути.

Логическая шпаргалка для Windows:
STARTUPINFOA si;
PROCESS_INFORMATION pi;
ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);
ZeroMemory(&pi, sizeof(pi));

if (CreateProcessA(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

Алгоритм работы вашего Mini-Shell
1. Чтение ввода: Считайте строку через `fgets` или `std::getline`.
2. Парсинг: Разбейте строку на отдельные слова по пробелам (используйте `strtok`).
3. Проверка на встроенные команды: `exit`, `pwd`, `help`.
4. Запуск внешней команды: Если слово не совпало, передаем массив аргументов в механизм создания процессов.

Дополнительно: Реализуйте команду смены директории `cd [путь]` с помощью `chdir()` (Linux) или `SetCurrentDirectoryA()` (Windows).
