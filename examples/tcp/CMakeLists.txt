cmake_minimum_required(VERSION 3.16)
project(mqtt_tcp_api_example)

add_executable(mqtt_tcp_api_example mqtt_tcp.c)
target_link_libraries(mqtt_tcp_api_example PRIVATE dpubsub)

