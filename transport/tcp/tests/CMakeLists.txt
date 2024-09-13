# Adiciona os arquivos de teste
add_executable(test_server server.c)
add_executable(test_client client.c)

# Liga os executáveis com as bibliotecas apropriadas
target_link_libraries(test_server PRIVATE tcp_server)
target_link_libraries(test_client PRIVATE tcp_client)
