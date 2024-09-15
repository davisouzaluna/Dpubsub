#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT 9090

int main() {
    int client_socket = create_connection_to_server("127.0.0.1", SERVER_PORT);
    if (client_socket < 0) {
        fprintf(stderr, "Failed to connect to the server\n");
        return EXIT_FAILURE;
    }
    const char *msg = "Hello, server!";
    size_t msg_len = strlen(msg);
    int send_msg = send_bytes_to_server(client_socket, msg, msg_len);

    close_connection_client(client_socket);
    return EXIT_SUCCESS;
}
