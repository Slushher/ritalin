cmake_minimum_required(VERSION 3.23.2)
project(RITALIN VERSION 1.0.0)

# debug flags
add_compile_options(-g)

# binary source
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

option(BUILD_EXAMPLE "Build the application with example code" OFF)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "lib/glad/glad.*" "lib/imgui/imgui*.*")

# create a static lib
add_library(ritalin_lib STATIC ${LIB_SOURCES})

target_include_directories(ritalin_lib PUBLIC headers lib/stb)
include_directories("lib/glad/include" "lib" "example")

target_link_libraries(ritalin_lib -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -g)

if(BUILD_EXAMPLE)
    add_compile_definitions(BUILD_EXAMPLE)
    file(GLOB_RECURSE EXAMPLE_SOURCES "example/*.cpp" "example/*.hpp")
    list(APPEND MAIN_SOURCES ${EXAMPLE_SOURCES})
else()
    set(MAIN_SOURCES "example/main.cpp")
endif()

add_executable(ritalin ${MAIN_SOURCES})
target_link_libraries(ritalin ritalin_lib)
