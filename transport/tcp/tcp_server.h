// tcp_server.h
#include <stddef.h>
#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#define INITIAL_BUFFER_SIZE 1024 // Tamanho inicial do buffer de recebimento

int create_connection_listen(int port);
int close_connection_server(int connfd);
int close_socket_server(int sockfd);
void receive_bytes_from_client(int connfd, void *buffer, size_t buffer_size);
//void *manage_buffer(void *buffer, size_t *buffer_size, size_t required_size);
void receive_bytes_with_dynamic_buffer(int connfd);
void receive_bytes_from_client_static_buff(int connfd, void *buffer, size_t buffer_size) ;

#endif // TCP_SERVER_H
