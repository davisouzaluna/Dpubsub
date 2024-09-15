#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PORT 9595
#define TEST_DATA_SIZE (1024 * 1024)  // 1 MB

// Função de teste de desempenho com buffer dinâmico
void performance_test_dynamic(int connfd) {
    clock_t start_time, end_time;
    double elapsed_time;

    // Inicia o cronômetro
    start_time = clock();

    // Recebe os dados com buffer dinâmico
    receive_bytes_with_dynamic_buffer(connfd);

    // Para o cronômetro
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Tempo decorrido (buffer dinâmico): %.6f segundos\n", elapsed_time);
    // Não há throughput para buffer dinâmico diretamente, pois não há controle sobre o tamanho do buffer
}

// Função de teste de desempenho com buffer estático
void performance_test_static(int connfd, size_t buffer_size) {
    // Aloca o buffer com o tamanho especificado
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        perror("Erro ao alocar memória para o buffer");
        exit(EXIT_FAILURE);
    }

    clock_t start_time, end_time;
    double elapsed_time;

    // Inicia o cronômetro
    start_time = clock();

    // Recebe os dados
    receive_bytes_from_client_static_buff(connfd, buffer, buffer_size);

    // Para o cronômetro
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Tempo decorrido (buffer estático): %.6f segundos\n", elapsed_time);
    printf("Throughput (buffer estático): %.2f MB/s\n", (buffer_size / 1024.0 / 1024.0) / elapsed_time);

    free(buffer);
}

int main() {
    int sockfd = create_and_listen_server(PORT);
    int connfd = accept_client(sockfd);

    // Testar a função com buffer estático
    printf("Testando com buffer estático:\n");
    performance_test_static(connfd, TEST_DATA_SIZE);

    // Testar a função com buffer dinâmico
    printf("Testando com buffer dinâmico:\n");
    performance_test_dynamic(connfd);

    close_connection_server(connfd);
    close_socket_server(sockfd); // Fechar o socket do servidor também
    return 0;
}
