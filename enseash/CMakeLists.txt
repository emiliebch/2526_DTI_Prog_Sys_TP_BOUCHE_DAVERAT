cmake_minimum_required(VERSION 4.0)
project(enseash C)

set(CMAKE_C_STANDARD 11)

add_executable(enseash main.c
        affiche_message.c
        execute_command.c
        manage_exit.c
        cmake-build-debug/return_code.c)
