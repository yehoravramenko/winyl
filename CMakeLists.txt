cmake_minimum_required(VERSION 3.15)
project(Winyl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# file(GLOB_RESOURCE sources src/*.cpp src/*.c)
file(GLOB sources src/winyl.cpp)
set(includes thirdparty/RGFW thirdparty/imgui thirdparty/imgui_imp_rgfw.h)

file(GLOB imgui_sources 
    thirdparty/imgui/imgui.cpp 
    thirdparty/imgui/imgui_draw.cpp 
    thirdparty/imgui/imgui_widgets.cpp 
    thirdparty/imgui/imgui_tables.cpp 
    thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui ${imgui_sources})
target_include_directories(imgui PRIVATE thirdparty/imgui)

add_executable(winyl ${sources})
target_link_libraries(winyl imgui)
target_link_libraries(winyl shlwapi)

set_property(TARGET winyl PROPERTY CXX_STANDARD 23)
set_property(TARGET winyl PROPERTY COMPILE_WARNING_AS_ERROR ON)

target_include_directories(winyl PRIVATE ${includes})

add_custom_command(TARGET winyl POST_BUILD
    COMMAND copy compile_commands.json ..
)