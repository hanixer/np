project(np)

include_directories(./)

add_library(np STATIC np.cpp np.h)

add_executable(libtest libtest.cpp  buffered_reader.cpp buffered_reader.h)
target_link_libraries(libtest np)

add_subdirectory(echo)
