# Adiciona os arquivos de teste
add_executable(test_server server.c)
add_executable(test_client client.c)
add_executable(recv_data_server_test recv_data_server_test.c)
add_executable(send_data_to_server_test send_data_to_server_test.c)


# Liga os executáveis com as bibliotecas apropriadas
target_link_libraries(test_server PRIVATE tcp_server)
target_link_libraries(test_client PRIVATE tcp_client)
target_link_libraries(recv_data_server_test PRIVATE tcp_server)
target_link_libraries(send_data_to_server_test PRIVATE tcp_client)
