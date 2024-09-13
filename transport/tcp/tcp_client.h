// tcp_client.h
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

int create_connection_server(char *addr, int port);
void close_connection(int sockfd);

#endif // TCP_CLIENT_H
