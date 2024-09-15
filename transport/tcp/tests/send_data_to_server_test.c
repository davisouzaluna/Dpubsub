#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define SERVER_IP "172.18.0.1"
#define SERVER_PORT 9595

// Inclua o código do cliente TCP aqui
#include "tcp_client.h"

// Função para testar o desempenho
void performance_test(const char *server_ip, int server_port, size_t data_size) {
    int sockfd;
    char *data;
    clock_t start_time, end_time;
    double elapsed_time;

    // Aloca memória para os dados de teste
    data = malloc(data_size);
    if (data == NULL) {
        perror("Error allocating memory for test data");
        exit(EXIT_FAILURE);
    }

    // Preenche o buffer com dados fictícios
    memset(data, 'A', data_size);

    // Cria conexão com o servidor
    sockfd = create_connection_to_server(server_ip, server_port);
    if (sockfd < 0) {
        free(data);
        exit(EXIT_FAILURE);
    }

    // Inicia o cronômetro
    start_time = clock();

    // Envia dados para o servidor
    send_bytes_to_server(sockfd, data, data_size);

    // Para o cronômetro
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Exibe os resultados
    printf("Data size: %zu bytes\n", data_size);
    printf("Elapsed time: %.6f seconds\n", elapsed_time);
    printf("Throughput: %.2f MB/s\n", (data_size / 1024.0 / 1024.0) / elapsed_time);

    // Fecha a conexão
    close_connection_client(sockfd);
    free(data);
}

int main() {
    // Configurações de teste
    const char *server_ip = SERVER_IP;
    int server_port = SERVER_PORT;
    size_t data_size = 1024 * 1024; // Tamanho dos dados em bytes (1 MB)

    performance_test(server_ip, server_port, data_size);

    return 0;
}
