cmake_minimum_required(VERSION 3.26)
project(tasks)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add the executable and source files
add_executable(
        tasks
        task.cpp
        main.cpp
        taskIndex.cpp
        repl.cpp
)
