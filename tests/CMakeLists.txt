cmake_minimum_required(VERSION 3.10)
project(tests C)

# Define o padrão C
set(CMAKE_C_STANDARD 99)

# Adiciona diretórios de inclusão
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../transport/tcp)

# Adiciona o executável de teste
add_executable(client_test client_test.c)

# Linka o executável de teste com as bibliotecas necessárias
target_link_libraries(client_test client tcp_client)
