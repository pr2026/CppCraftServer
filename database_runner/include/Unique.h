#pragma once
#include <chrono>
#include <random>
#include <string>

inline std::string GenerateUniqueId() {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch()
    )
                  .count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    return std::to_string(ms) + "_" + std::to_string(dis(gen));
}
