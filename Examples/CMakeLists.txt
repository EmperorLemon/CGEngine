cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} "Example01/Example01.cpp")

target_link_libraries(${PROJECT_NAME} PUBLIC CGCore)