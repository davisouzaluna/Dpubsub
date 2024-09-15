#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int port = 9090;
    int sockfd = create_and_listen_server(port);
    int connfd = accept_client(sockfd);

    

    // Fecha a conexão e o socket
    close_connection_server(connfd);
    close_socket_server(sockfd);


    return 0;
}
