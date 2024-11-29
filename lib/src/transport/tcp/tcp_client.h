// tcp_client.h
#include <stddef.h>
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H


int create_connection_to_server(const char *addr, int port);
int receive_bytes_from_server_static_buff(int sockfd, void *buffer, size_t buffer_size);
int close_connection_client(int sockfd);
int send_bytes_to_server(int sockfd, const void *data, size_t data_len);

#endif // TCP_CLIENT_H
