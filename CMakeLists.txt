cmake_minimum_required(VERSION 3.0)
project(enigma-chess)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
)

link_directories()

add_executable(enigma-chess ${SOURCES})