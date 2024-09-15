#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int create_and_listen_server(int port) {
    int sockfd;
    struct sockaddr_in serv_addr;
    int optval = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("Erro ao configurar socket para reutilização de endereço");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro ao fazer o bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) < 0) {
        perror("Erro ao escutar");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor ouvindo na porta %d\n", port);
    return sockfd;
}

// Aceita uma conexão do cliente. A funcao retorna o descritor do socket da conexao
int accept_client(int sockfd) {
    int connfd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (connfd < 0) {
        perror("Erro ao aceitar a conexão");
        return -1;
    }

    printf("Cliente conectado!\n");
    return connfd;
}
// Função que gerencia o buffer de forma dinâmica(pra evitar o estouro de buffer)
void *manage_buffer(void *buffer, size_t *buffer_size, size_t required_size) {
    // Verifica se o buffer precisa ser expandido
    if (required_size > *buffer_size) {
        printf("Expanding buffer from %zu to %zu bytes\n", *buffer_size, required_size);
        void *new_buffer = realloc(buffer, required_size);  // Expande o buffer
        if (new_buffer == NULL) {
            perror("Error reallocating memory");
            free(buffer);
            exit(EXIT_FAILURE);
        }
        buffer = new_buffer;
        *buffer_size = required_size; // Atualiza o tamanho do buffer
    }
    return buffer;
}

/* Função que recebe os dados e ajusta o buffer dinamicamente. Não é necessário fornecer um buffer.
Essa função aloca memória para o buffer e libera a memória quando termina.
Com relação ao desempenho, essa função é menos eficiente do que receber os dados em um buffer estático.
Utilize essa função quando não souber o tamanho dos dados a serem recebidos, ou como debug.
REITERANDO, não é recomendado para uso em produção, pois alocar e liberar memória é custoso.
*/
void receive_bytes_with_dynamic_buffer(int connfd) {
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    void *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    size_t total_bytes_received = 0;
    int bytes_received;
    size_t chunk_size = 512;  // Tamanho do bloco de recepção

    while (1) {
        // Verifica se o buffer é grande o suficiente para o próximo chunk
        buffer = manage_buffer(buffer, &buffer_size, total_bytes_received + chunk_size);

        // Recebe os dados
        bytes_received = recv(connfd, (char *)buffer + total_bytes_received, chunk_size, 0);

        if (bytes_received < 0) {
            perror("Error receiving bytes");
            break;
        } else if (bytes_received == 0) {
            printf("Client disconnected\n");
            break;
        }

        total_bytes_received += bytes_received;
        printf("Received %d bytes, total received: %zu\n", bytes_received, total_bytes_received);
    }

    // Processar os dados recebidos(debug)
    printf("Total data received: %zu bytes\n", total_bytes_received);

    // Liberar a memória do buffer
    free(buffer);
}

/*
Os dados recebidos do cliente são armazenados no buffer fornecido.
O tamanho do buffer é fornecido como argumento.
LEMBRE-SE: O buffer deve ser grande o suficiente para armazenar os dados recebidos.
*/
void receive_bytes_from_client_static_buff(int connfd, void *buffer, size_t buffer_size) {
    int bytes_received = recv(connfd, buffer, buffer_size, 0);
    if (bytes_received < 0) {
        perror("Error receiving bytes");
    } else if (bytes_received == 0) {
        printf("Client disconnected\n");
    } else {
        printf("Received %d bytes from client\n", bytes_received);
    }
}


void send_bytes_to_client(int connfd, const void *buffer, size_t buffer_size) {
    ssize_t bytes_sent = send(connfd, buffer, buffer_size, 0);
    if (bytes_sent < 0) {
        perror("Error sending bytes to client");
    } else {
        printf("Sent %zu bytes to client\n", (size_t)bytes_sent);
    }
}

int close_connection_server(int connfd) {
    if (close(connfd) < 0) {
        perror("Erro ao fechar a conexão com o cliente");
        return -1;  // Retorna -1 em caso de erro
    }
    return 0;  // Retorna 0 em caso de sucesso
}


int close_socket_server(int sockfd) {
    if (close(sockfd) < 0) {
        perror("Erro ao fechar o socket do servidor");
        return -1;
    }
    return 0;
}


int close_resource(int fd) {
    if (close(fd) < 0) {
        perror("Erro ao fechar o recurso");
        return -1;
    }
    return 0;
}