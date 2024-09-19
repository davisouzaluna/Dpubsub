cmake_minimum_required(VERSION 3.10)
project(my_project C)

# Define o padrão C
set(CMAKE_C_STANDARD 99)

# Adiciona diretórios de inclusão
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/transport/tcp)

# Biblioteca client
add_library(client STATIC client.c)

# Biblioteca tcp_client
add_library(tcp_client STATIC transport/tcp/tcp_client.c)
add_library (packets STATIC packets.c)

# Adiciona o diretório de testes
add_subdirectory(tests)
