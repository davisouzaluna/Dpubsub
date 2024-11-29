cmake_minimum_required(VERSION 3.10)
project(myapp)
find_package(dpubsub REQUIRED)

add_executable(my_app mqtt_tcp.c)
target_link_libraries(my_app PRIVATE dpubsub)

