#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//#define SERVER_PORT 8080  // Porta estática

int create_connection_listen(int port){

    int sockfd, connfd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Cria o socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    // Inicializa a estrutura de endereço
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);  // Porta estática

    // Faz o bind
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro ao fazer o bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Escuta por conexões
    if (listen(sockfd, 5) < 0) {
        perror("Erro ao escutar");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor ouvindo na porta %d\n", port);

    // Aceita uma conexão
    connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (connfd < 0) {
        perror("Erro ao aceitar a conexão");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Cliente conectado!\n");
}

//Fecha somente a conexão com o cliente
int close_connection(int confd){
    close(confd);
}

//Fecha o socket
int close_socket(int sockfd){
    close(sockfd);
}
