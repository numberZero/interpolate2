cmake_minimum_required(VERSION 2.6)
project(interpolate2)

add_executable(interpolate2 main.cpp)

install(TARGETS interpolate2 RUNTIME DESTINATION bin)
