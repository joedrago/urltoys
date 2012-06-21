cmake_minimum_required(VERSION 2.8)

if(UNIX)
    add_definitions(-g)
endif()

add_subdirectory(lib)
add_subdirectory(apps)

