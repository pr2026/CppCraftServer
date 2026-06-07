#include "Error.h"

std::string error(int status) {
    if (status == 0) {
        return "Успешно :)";
    }
    if (status > 128) {
        int signal = status - 128;
        switch (signal) {
            case 1:
                return "Hangup";
            case 2:
                return "Interrupt";
            case 3:
                return "Quit";
            case 4:
                return "Illegal instruction";
            case 5:
                return "Trace trap";
            case 6:
                return "Abort";
            case 8:
                return "Floating point exception";
            case 9:
                return "Killed";
            case 11:
                return "Segmentation fault";
            case 13:
                return "Broken pipe";
            case 15:
                return "Terminated";
            default:
                return "Сигнал " + std::to_string(signal);
        }
    }

    switch (status) {
        case 1:
            return "General error";
        case 2:
            return "No such file or directory";
        case 126:
            return "Command cannot execute";
        case 127:
            return "Command not found";
        case 128:
            return "Invalid exit argument";
        default:
            return "Error code " + std::to_string(status);
    }
}
