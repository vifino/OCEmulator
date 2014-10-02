cmake_minimum_required(VERSION 2.6)
project(ocemulator)

add_executable(ocemulator main.cpp)

install(TARGETS ocemulator RUNTIME DESTINATION bin)
