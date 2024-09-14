// test_server.c
#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int connfd = create_connection_listen(8080);
    if (connfd < 0) {
        fprintf(stderr, "Erro ao criar conexão de escuta\n");
        exit(EXIT_FAILURE);
    }

    printf("Teste do servidor concluído com sucesso.\n");

    close_connection_server(connfd);
    close_socket_server(8080);

    return 0;
}
