#pragma once
#include <string>

struct Task {
    int id = -1;
    std::string title;
    std::string description;
    std::string difficulty;
    int created_by = -1;

    Task() : id(0) {
    }

    Task(
        int id,
        const std::string &title,
        const std::string &description,
        const std::string &difficulty,
        int created_by = -1
    )
        : id(id),
          title(title),
          description(description),
          difficulty(difficulty),
          created_by(created_by) {
    }
};
