cmake_minimum_required(VERSION 3.10)
project(Winyl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# file(GLOB_RESOURCE sources src/*.cpp src/*.c)
set(sources src/winyl.cpp)

set(includes thirdparty/RGFW)

add_executable(winyl ${sources})

set_property(TARGET winyl PROPERTY CXX_STANDARD 17)
set_property(TARGET winyl PROPERTY COMPILE_WARNING_AS_ERROR ON)

target_include_directories(winyl PRIVATE ${includes})

add_custom_command(TARGET winyl POST_BUILD
    COMMAND copy compile_commands.json ..
)