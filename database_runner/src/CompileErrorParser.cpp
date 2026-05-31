#include <string>

std::string parseCompileError(const std::string& raw_output) {
     if (raw_output.find("does not name a type") != std::string::npos) {
        return "Ошибка: неизвестный тип";
    }
    if (raw_output.find("was not declared") != std::string::npos) {
        return "Ошибка: переменная или функция не объявлена";
    }
    if (raw_output.find("undefined reference") != std::string::npos) {
        return "Ошибка: неопределённая ссылка";
    }
    if (raw_output.find("No such file or directory") != std::string::npos) {
        return "Ошибка: файл не найден";
    }
}
