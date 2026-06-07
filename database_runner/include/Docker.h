#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include "Terminal.h"


#ifdef _WIN32
    #define IS_WINDOWS 1
#elif __APPLE__
    #define IS_MACOS 1
#else
    #define IS_LINUX 1
#endif

inline std::string getTempPath() {
    #ifdef _WIN32
        const char* user = std::getenv("USERNAME");
        if (user) {
            return "C:/Users/" + std::string(user) + "/AppData/Local/Temp";
        }
        return "C:/Temp";
    #else
        return "/tmp";
    #endif
}
inline std::string getDockerImage() {
    #ifdef __APPLE__
        return "arm64v8/gcc:latest";
    #else
        return "gcc:latest";
    #endif
}

inline void
CreateTempFile(const std::string &unique_id, const std::string &code) {
    std::string path = "/tmp/solution_" + unique_id + ".cpp";  // way to file
    std::ofstream file(path);
    file << code;
    file.close();
}

inline void RemoveTempFiles(const std::string &unique_id) {
    std::filesystem::remove("/tmp/solution_" + unique_id + ".cpp");
    std::filesystem::remove("/tmp/solution_" + unique_id);
}

inline Terminal_result compile(const std::string &unique_id) {
    std::string image = getDockerImage();
    std::string temp_path = getTempPath();
    std::string mount = "-v " + temp_path + ":/workspace";

    std::string compile_command =
        "docker run --rm "
        "--memory=256m "
        "--cpus=0.5 "
        "--stop-timeout=5 "
        "--network=none "
        "--read-only "
        "-v /tmp:/workspace "
        "-w /workspace "
        + image + " g++ /workspace/solution_" +
        unique_id + ".cpp -o solution_" + unique_id + " 2>&1";


    return terminal(compile_command);
}

inline Terminal_result
run(const std::string &unique_id, const std::string &input) {
    std::string image = getDockerImage();
    std::string temp_path = getTempPath();
    std::string mount = "-v " + temp_path + ":/workspace";

    
    std::string run_result =
        "docker run --rm -i "
        "--memory=256m "
        "--cpus=0.5 "
        "--network=none "
        "--read-only "
        "--stop-timeout=5 "
        + mount + " "
        "-w /workspace "
        + image + " ./solution_" +
        unique_id;
    return terminal(run_result, input);
}