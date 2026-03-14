#pragma once
#include <string>

struct Task {
    int id;                
    std::string title;      
    std::string description; 
    std::string difficulty;   
    
    Task() : id(0) {}
    Task(int id, const std::string& title, const std::string& description, 
         const std::string& difficulty): id(id), title(title), description(description), 
          difficulty(difficulty) {}
};