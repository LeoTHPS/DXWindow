cmake_minimum_required(VERSION 3.24)

set(CMAKE_C_STANDARD   17)
set(CMAKE_CXX_STANDARD 20)

project(DXWindow)
add_library(DXWindow DXWindow.cpp)
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
target_include_directories(DXWindow PUBLIC ${CMAKE_CURRENT_LIST_DIR})
