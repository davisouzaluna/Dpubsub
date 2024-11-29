cmake_minimum_required(VERSION 3.10)
project(tcp_project C)

# Define o padrão C
set(CMAKE_C_STANDARD 99)

# Adiciona diretórios de inclusão
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Biblioteca tcp_server
add_library(tcp_server STATIC tcp_server.c)

# Biblioteca tcp_client
add_library(tcp_client STATIC tcp_client.c)

# Adiciona o diretório de testes
add_subdirectory(tests)
