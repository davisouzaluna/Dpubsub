cmake_minimum_required(VERSION 3.10)
project(tests)
find_package(dpubsub REQUIRED)

add_executable(client_test client_test.c)
add_executable(api_test api_test.c)



target_link_libraries(client_test dpubsub)
target_link_libraries(api_test dpubsub)