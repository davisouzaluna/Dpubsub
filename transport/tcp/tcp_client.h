// tcp_client.h
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

int create_connection_to_server(char *addr, int port);
void close_connection_client(int sockfd);

#endif // TCP_CLIENT_H
