cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code error.cpp main.cpp book.cpp user.cpp token_scanner.cpp global.cpp log.cpp)
