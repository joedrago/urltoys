include_directories(${CMAKE_SOURCE_DIR}/lib)

set(SRCS
    utConsole.c
    utConsole.h
)

add_executable(utconsole ${SRCS})
target_link_libraries(utconsole urltoys readline)
