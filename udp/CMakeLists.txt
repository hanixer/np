project(udp)

set(${PROJECT_NAME}_headers udp_common.h)

add_executable(udpserv udpserv.cpp ${${PROJECT_NAME}_headers})
add_executable(udpcli udpcli.cpp ${${PROJECT_NAME}_headers})

target_link_libraries(udpserv np)
target_link_libraries(udpcli np)
