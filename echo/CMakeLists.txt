project(echo)

set(${PROJECT_NAME}_headers)
set(${PROJECT_NAME}_sources)

add_executable(servtest servtest.cpp)
target_link_libraries(servtest np)