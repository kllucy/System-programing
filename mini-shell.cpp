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
