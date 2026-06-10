cmake_minimum_required(VERSION 3.10)
project(CppCraftRunner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

find_package(SQLite3 REQUIRED)

set(SOURCES
    src/Database.cpp
    src/UserDB.cpp
    src/TaskDB.cpp
    src/SolutionDB.cpp
    src/StatisticsDB.cpp
    src/run_student_code.cpp
    src/terminal.cpp
    src/error.cpp
    src/main.cpp
)

add_executable(program ${SOURCES})
target_link_libraries(program SQLite::SQLite3)

add_executable(create_table src/CreateTable.cpp src/Database.cpp)
target_link_libraries(create_table SQLite::SQLite3)


#option(BUILD_TESTS "Build unit tests" OFF)
#if(BUILD_TESTS)
#    add_executable(test_runner tests/test_runner.cpp ${SOURCES})
#    target_link_libraries(test_runner SQLite::SQLite3)
#endif()
