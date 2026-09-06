cmake_minimum_required(VERSION 3.24)

set(CMAKE_C_STANDARD   17)
set(CMAKE_CXX_STANDARD 20)

include(FIO.cmake)   # optional but required for loading DXWindow::Sound from file. will be removed in the future
include(ImGui.cmake) # optional

include(DXWindow.cmake)

project(demo)
add_executable(demo demo.cpp)
target_link_libraries(demo DXWindow)
