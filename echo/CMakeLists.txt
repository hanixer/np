project(echo)

set(${PROJECT_NAME}_headers echo_header.h)
set(${PROJECT_NAME}_sources)

add_executable(servtest servtest.cpp ${${PROJECT_NAME}_headers})
target_link_libraries(servtest np)

add_executable(echoserv echoserv.cpp ${${PROJECT_NAME}_headers})
target_link_libraries(echoserv np)

add_executable(echoserv01 echoserv01.cpp ${${PROJECT_NAME}_headers})
target_link_libraries(echoserv01 np)

add_executable(echoserv02 echoserv02.cpp ${${PROJECT_NAME}_headers})
target_link_libraries(echoserv02 np)

#add_executable(echoservsum echoservsum.cpp ${${PROJECT_NAME}_headers})
#target_link_libraries(echoservsum np)

add_executable(echocli echocli.cpp ${${PROJECT_NAME}_headers})
target_link_libraries(echocli np)

add_executable(echocli02 echocli02.cpp ${${PROJECT_NAME}_headers})
target_link_libraries(echocli02 np)
