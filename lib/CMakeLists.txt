set(SRCS
    utActionsCore.c
    utActionsCore.h
    utArray.c
    utArray.h
    utContext.c
    utContext.h
    utList.c
    utList.h
    utString.c
    utString.h
)

add_library(urltoys ${SRCS})
target_link_libraries(urltoys pcre)

