cmake_minimum_required(VERSION 3.23.2)

project(RITALIN VERSION 1.0.0)

add_compile_options(-g)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")
option(BUILD_EXAMPLE "Build the library example" OFF)
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" "lib/glad/glad.*" "lib/imgui/imgui*.*")
add_executable(ritalin ${SOURCES})

target_include_directories(ritalin PUBLIC headers)
include_directories("lib/glad/include" "lib")
target_link_libraries(ritalin -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -g)

if(BUILD_EXAMPLE)
    file(GLOB_RECURSE EXAMPLE_SOURCES "examples/*.cpp")
    add_executable(ritalin_example ${SOURCES})
    target_link_libraries(ritalin_example ritalin_lib)
    target_include_directories(ritalin_example PUBLIC headers)
    include_directories("lib/glad/include" "lib")
endif()