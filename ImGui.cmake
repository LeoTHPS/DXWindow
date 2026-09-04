project(imgui)

include(FetchContent)

# set(FREETYPE_GIT     "https://gitlab.freedesktop.org/freetype/freetype.git")
# set(FREETYPE_GIT_TAG "264b5fbf5b912b39f98d038bf75d39be0a73f21b") # 2.14.2
# FetchContent_Declare(freetype_external GIT_REPOSITORY ${FREETYPE_GIT} GIT_TAG ${FREETYPE_GIT_TAG} EXCLUDE_FROM_ALL)
# FetchContent_MakeAvailable(freetype_external)

set(IMGUI_GIT     "https://github.com/ocornut/imgui.git")
set(IMGUI_GIT_TAG "6ded5230d043aa32c755e65c910c2af5002fb9f9") # v1.92.6
FetchContent_Declare(imgui_external GIT_REPOSITORY ${IMGUI_GIT} GIT_TAG ${IMGUI_GIT_TAG} EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(imgui_external)
add_library(imgui STATIC ${imgui_external_SOURCE_DIR}/imgui.cpp ${imgui_external_SOURCE_DIR}/imgui_demo.cpp ${imgui_external_SOURCE_DIR}/imgui_draw.cpp ${imgui_external_SOURCE_DIR}/imgui_tables.cpp ${imgui_external_SOURCE_DIR}/imgui_widgets.cpp)
target_sources(imgui PRIVATE ${imgui_external_SOURCE_DIR}/backends/imgui_impl_win32.cpp ${imgui_external_SOURCE_DIR}/backends/imgui_impl_dx11.cpp)
target_link_libraries(imgui PRIVATE Imm32 Dwmapi d3d11 d3dcompiler)
target_compile_definitions(imgui PUBLIC -DIMGUI_ENABLE_WIN32_DEFAULT_IME_FUNCTIONS=1)
target_include_directories(imgui PUBLIC ${imgui_external_SOURCE_DIR} ${imgui_external_SOURCE_DIR}/backends/)

if(TARGET freetype)
	target_sources(imgui PRIVATE ${imgui_external_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp)
	target_link_libraries(imgui PRIVATE freetype)
	target_compile_definitions(imgui PUBLIC -DIMGUI_ENABLE_FREETYPE=1)
	target_include_directories(imgui PUBLIC ${imgui_external_SOURCE_DIR}/misc/freetype/ ${freetype_external_SOURCE_DIR}/include/)
endif()
