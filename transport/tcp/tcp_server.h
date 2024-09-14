// tcp_server.h
#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#define INITIAL_BUFFER_SIZE 1024 // Tamanho inicial do buffer de recebimento

int create_connection_listen(int port);
int close_connection_server(int connfd);
int close_socket_server(int sockfd);
void receive_bytes_from_client(int connfd, void *buffer, size_t buffer_size);
#endif // TCP_SERVER_H
