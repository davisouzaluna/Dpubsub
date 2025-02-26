cmake_minimum_required(VERSION 3.16)
project(ngtcp2_exemplo)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NGTCP2 REQUIRED libngtcp2)
pkg_check_modules(NGTCP2_CRYPTO REQUIRED libngtcp2_crypto_wolfssl)
pkg_check_modules(WOLFSSL REQUIRED wolfssl)
pkg_check_modules(OPENSSL REQUIRED openssl)
pkg_check_modules(NGTCP2_CRYPTO_OPENSSL REQUIRED libngtcp2_crypto_quictls)
pkg_check_modules(QUIC_TLS REQUIRED libcrypto)

#criar os executáveis
add_executable(quic_client quic_client_ngtcp2.c)
add_executable(quic_srv quic_srv_ngtcp2.c)
add_executable(test_openssl test_client.c)

# Incluir diretórios das bibliotecas
target_include_directories(quic_client PRIVATE
    ${NGTCP2_INCLUDE_DIRS}
    ${NGTCP2_CRYPTO_INCLUDE_DIRS}
    ${WOLFSSL_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
)

# Linkar as bibliotecas necessárias
target_link_libraries(quic_client
    ${NGTCP2_LIBRARIES}
    ${NGTCP2_CRYPTO_LIBRARIES}   # Adiciona ngtcp2_crypto_wolfssl
    ${WOLFSSL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
)




target_include_directories(quic_srv PRIVATE
    ${NGTCP2_INCLUDE_DIRS}
    ${NGTCP2_CRYPTO_INCLUDE_DIRS}
    ${WOLFSSL_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
)


target_link_libraries(quic_srv
    ${NGTCP2_LIBRARIES}
    ${NGTCP2_CRYPTO_LIBRARIES}   # Adiciona ngtcp2_crypto_wolfssl
    ${WOLFSSL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
)

target_include_directories(test_openssl PRIVATE
    ${NGTCP2_INCLUDE_DIRS}
    ${NGTCP2_CRYPTO_INCLUDE_DIRS}
    ${WOLFSSL_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
    ${NGTCP2_CRYPTO_OPENSSL} # Adiciona ngtcp2_crypto_quictls
)


target_link_libraries(test_openssl
    ${NGTCP2_LIBRARIES}
    ${NGTCP2_CRYPTO_LIBRARIES}   # Adiciona ngtcp2_crypto_wolfssl
    ${WOLFSSL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${NGTCP2_CRYPTO_OPENSSL}
)
