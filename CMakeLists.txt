cmake_minimum_required(VERSION 3.10)
project(Editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
include_directories(src)


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_executable(editor 
    src/main.cpp 
    src/editor.cpp
    src/fileIO.cpp
)

target_link_libraries(editor ${CURSES_LIBRARIES} )

add_custom_command(TARGET editor POST_BUILD
    COMMAND ./editor
    COMMENT "RUNNING"
)