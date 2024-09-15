#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int port = 9090;
    int sockfd = create_and_listen_server(port);
    int connfd = accept_client(sockfd);

    char buffer[1024];
    size_t buffer_size = sizeof(buffer);
    int bytes_received = receive_bytes_from_client_static_buff(connfd,buffer, buffer_size) ;

    if (bytes_received > 0) {
    // Adiciona um terminador nulo ao buffer para que possa ser impresso como string
    buffer[bytes_received] = '\0';
    printf("Data received from client: %s\n", buffer);
    } else if (bytes_received == 0) {
        printf("No data received from client.\n");
    } else {
        printf("Error receiving data from client.\n");
    }
    // Fecha a conexão e o socket
    close_connection_server(connfd);
    close_socket_server(sockfd);


    return 0;
}
