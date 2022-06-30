cmake_minimum_required(VERSION 3.17)
project(On_Line_Music_Server C)

set(CMAKE_C_STANDARD 99)

add_executable(On_Line_Music_Server Music_Server_main.c Server_Functions.c Music_Server.h)