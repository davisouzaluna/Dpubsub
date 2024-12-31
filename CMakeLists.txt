cmake_minimum_required(VERSION 3.16)
project(dpubsub LANGUAGES C)


set(LIB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/src")


set(LIB_PUBLIC_INCLUDE_DIR "${LIB_SRC_DIR}/include")


set(LIB_PRIVATE_INCLUDE_DIR "${LIB_SRC_DIR}")

set(LIB_SOURCES
    ${LIB_SRC_DIR}/include/dpubsub.c  
    ${LIB_SRC_DIR}/client.c
    ${LIB_SRC_DIR}/client_tcp_api.c
    ${LIB_SRC_DIR}/packets.c
    ${LIB_SRC_DIR}/transport/tcp/tcp_client.c
)

set(LIB_HEADERS_PRIVATE
    ${LIB_SRC_DIR}/include/dpubsub.h
    ${LIB_SRC_DIR}/client.h
    ${LIB_SRC_DIR}/client_tcp_api.h
    ${LIB_SRC_DIR}/packets.h
    ${LIB_SRC_DIR}/transport/tcp/tcp_client.h
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})


target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${LIB_PUBLIC_INCLUDE_DIR}> 
        $<INSTALL_INTERFACE:include>                
    PRIVATE
        ${LIB_PRIVATE_INCLUDE_DIR}                   
        ${LIB_SRC_DIR}/transport/tcp                 
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# #####################################################################
#
#TODO: Install feature( Won't work for now)
#
#install(
#    FILES ${LIB_PUBLIC_INCLUDE_DIR}/dpubsub.h
#    DESTINATION include
#)
#install(
#    TARGETS ${PROJECT_NAME}
#    EXPORT dpubsubTargets
#    ARCHIVE DESTINATION lib
#    LIBRARY DESTINATION lib
#    RUNTIME DESTINATION bin
#)

#install(
#    EXPORT dpubsubTargets
#    NAMESPACE dpubsub
#    DESTINATION lib/cmake/dpubsub
#)

#include(CMakePackageConfigHelpers)

#configure_package_config_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/dpubsubConfig.cmake.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/dpubsubConfig.cmake"
#    INSTALL_DESTINATION lib/cmake/dpubsub
#)
#install(
#    FILES "${CMAKE_CURRENT_BINARY_DIR}/dpubsubConfig.cmake"
#    DESTINATION lib/cmake/dpubsub
#)

# #####################################################################

add_subdirectory(examples/tcp)
add_subdirectory(lib/src/transport/tcp)
add_subdirectory(tests)