#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 8080

int main() {
    int client_socket = create_connection_to_server("127.0.0.1", SERVER_PORT);
    if (client_socket < 0) {
        fprintf(stderr, "Failed to connect to the server\n");
        return EXIT_FAILURE;
    }

    // A lógica do cliente pode incluir o envio e recebimento de dados.
    // Aqui, o cliente apenas se conecta e fecha a conexão.

    close_connection_client(client_socket);
    return EXIT_SUCCESS;
}
