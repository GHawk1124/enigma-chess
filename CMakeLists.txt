cmake_minimum_required(VERSION 3.0)
project(enigma-chess)
set(CMAKE_CXX_STANDARD 11)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

link_directories()

add_executable(enigma-chess ${SOURCES})