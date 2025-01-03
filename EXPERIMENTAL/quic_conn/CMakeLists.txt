cmake_minimum_required(VERSION 3.16)
project(ngtcp2_exemplo)

set(CMAKE_CXX_STANDARD 17)

set(
    ngtcp2_crypto_wolfssl_static
    ngtcp2
)

add_executable( quic_client quic_client_ngtcp2.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NGTCP2 REQUIRED libngtcp2)
pkg_check_modules(OPENSSL REQUIRED openssl)

if (NOT LIBEV_FOUND)
    set(LIBEV_INCLUDE_DIRS "/usr/include")
    set(LIBEV_LIBRARIES "/usr/lib/x86_64-linux-gnu/libev.so")
endif()

target_include_directories(quic_client PRIVATE
    ${NGTCP2_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
    ${LIBEV_INCLUDE_DIRS}
)

target_link_libraries(quic_client
    ${NGTCP2_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${LIBEV_LIBRARIES}
)
message(STATUS "NGTCP2 include dirs: ${NGTCP2_INCLUDE_DIRS}")
message(STATUS "OpenSSL include dirs: ${OPENSSL_INCLUDE_DIRS}")
message(STATUS "libev include dirs: ${LIBEV_INCLUDE_DIRS}")
message(STATUS "NGTCP2 libraries: ${NGTCP2_LIBRARIES}")