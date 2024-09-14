// tcp_server.h
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

int create_connection_listen(int port);
int close_connection_server(int connfd);
int close_socket_server(int sockfd);

#endif // TCP_SERVER_H
