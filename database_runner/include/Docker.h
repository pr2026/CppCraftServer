#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include "Terminal.h"

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
    std::string compile_command =
        "docker run --rm "
        "--memory=256m "
        "--cpus=0.5 "
        "--stop-timeout=5 "
        "--network=none "
        "--read-only "
        "-v /tmp:/workspace "
        "-w /workspace "
        "arm64v8/gcc:latest g++ /workspace/solution_" +
        unique_id + ".cpp -o solution_" + unique_id + " 2>&1";

    return terminal(compile_command);
}

inline Terminal_result
run(const std::string &unique_id, const std::string &input) {
    std::string run_result =
        "docker run --rm -i "
        "--memory=256m "
        "--cpus=0.5 "
        "--network=none "
        "--read-only "
        "--stop-timeout=5 "
        "-v /tmp:/workspace "
        "-w /workspace "
        "arm64v8/gcc:latest ./solution_" +
        unique_id;
    return terminal(run_result, input);
}