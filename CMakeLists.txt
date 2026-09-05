cmake_minimum_required(VERSION 3.24)

set(CMAKE_C_STANDARD   17)
set(CMAKE_CXX_STANDARD 20)

# FIO can be removed but it will remove support for loading DXWindow::Sound from file
include(FIO.cmake)

# ImGui can be removed
include(ImGui.cmake)

project(DXWindow)
add_library(DXWindow
	DXWindow.cpp
)
target_link_libraries(DXWindow PRIVATE
	d2d1
	d3d11
	gdi32
	winmm
	dwrite
	dxguid
	dsound
	Comdlg32
	windowscodecs
)
target_include_directories(DXWindow PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
)
if(TARGET FIO)
	target_link_libraries(DXWindow PUBLIC FIO)
	target_compile_definitions(DXWindow PUBLIC DXWINDOW_FIO=1)
endif()
if(TARGET imgui)
	target_link_libraries(DXWindow PUBLIC imgui)
	target_compile_definitions(DXWindow PUBLIC DXWINDOW_IMGUI=1)
endif()
