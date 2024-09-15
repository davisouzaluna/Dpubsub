#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"  
#define SERVER_PORT 8080       

// Cria uma conexão com o servidor
int create_connection_to_server(const char *addr, int port) {
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

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);  

    // Converte o IP do servidor
    if (inet_pton(AF_INET, addr, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(sockfd);
        return -1; 
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

/*Usa o metodo send para enviar os dados para o servidor.
O dado para qualquer tipo de dado, e data_len é o tamanho do dado.
*/
int send_bytes_to_server(int sockfd, const void *data, size_t data_len) {
    size_t total_bytes_sent = 0;
    const char *data_ptr = data;

    while (total_bytes_sent < data_len) {
        int bytes_sent = send(sockfd, data_ptr + total_bytes_sent, data_len - total_bytes_sent, 0);
        if (bytes_sent < 0) {
            perror("Error sending bytes");
            break;
        }
        total_bytes_sent += bytes_sent;
    }

    printf("Sent %zu bytes to server\n", total_bytes_sent);
    return 0;
}

int receive_bytes_from_server_static_buff(int sockfd, void *buffer, size_t buffer_size) {
    // Recebe dados do cliente
    ssize_t bytes_received = recv(sockfd, buffer, buffer_size, 0);
    if (bytes_received < 0) {
        perror("Error receiving bytes");
        return -1; // Retorna -1 em caso de erro
    } else if (bytes_received == 0) {
        printf("Client disconnected\n");
        return 0; // Retorna 0 quando o cliente se desconectar
    } else {
        // remover esse printf se não for mais necessário
        printf("Received %ld bytes from client\n", (long)bytes_received);
        return (int)bytes_received; // Retorna o número de bytes recebidos
    }
}
// Fecha a conexão
int close_connection_client(int sockfd) {
    if(close(sockfd) < 0) {
        perror("Error closing connection");
        return -1;
    }
    return 0;
}

