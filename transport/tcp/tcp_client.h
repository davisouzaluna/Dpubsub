// tcp_client.h
#include <stddef.h>
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

int create_connection_to_server(const char *addr, int port);
void close_connection_client(int sockfd);
void send_bytes_to_server(int sockfd, const void *data, size_t data_len);

#endif // TCP_CLIENT_H
