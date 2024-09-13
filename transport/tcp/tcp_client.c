#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"  
#define SERVER_PORT 8080       

// Cria uma conexão com o servidor
int create_connection_server(char *addr, int port) {
    int sockfd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Cria o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return -1; // Retorna erro em vez de sair
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = htons(0);  // Porta dinâmica escolhida pelo SO

    // Faz o bind para a porta dinâmica
    if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("Error when binding the client");
        close(sockfd);
        return -1; // Retorna erro em vez de sair
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);  

    // Converte o IP do servidor
    if (inet_pton(AF_INET, addr, &serv_addr.sin_addr) <= 0) {
        perror("Error converting server IP");
        close(sockfd);
        return -1; // Retorna erro em vez de sair
    }

    // Conecta ao servidor
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to the server");
        close(sockfd);
        return -1; // Retorna erro em vez de sair
    }

    printf("Connected to the server on port %d\n", port);

    // Obtém a porta dinâmica atribuída ao cliente
    if (getsockname(sockfd, (struct sockaddr *)&client_addr, &client_len) == -1) {
        perror("Error getting dynamic port from client");
        close(sockfd);
        return -1; // Retorna erro em vez de sair
    }

    printf("Dynamic port assigned to the client: %d\n", ntohs(client_addr.sin_port));
    return sockfd; // Retorna o socket fd para uso posterior
}

// Fecha a conexão
void close_connection(int sockfd) {
    close(sockfd);
}
