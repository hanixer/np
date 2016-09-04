project(echo)

set(${PROJECT_NAME}_headers echo_header.h)
set(${PROJECT_NAME}_sources)

add_executable(servtest servtest.cpp ${${PROJECT_NAME}_headers})
target_link_libraries(servtest np)

add_executable(echoserv echoserv.cpp ${${PROJECT_NAME}_headers})
target_link_libraries(echoserv np)

add_executable(echocli echocli.cpp ${${PROJECT_NAME}_headers})
target_link_libraries(echocli np)
