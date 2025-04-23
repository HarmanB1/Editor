cmake_minimum_required(VERSION 3.10)
project(Editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Curses REQUIRED)

add_executable(editor 
    main.cpp 
    editor.cpp
)

target_link_libraries(editor ${CURSES_LIBRARIES})