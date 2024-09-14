#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PORT 8080
#define INITIAL_BUFFER_SIZE 1024 * 64 // 64 KB
#define TEST_DATA_SIZE (1024 * 1024)  // 1 MB

// Função de teste de desempenho
void performance_test(int connfd, void (*receive_function)(int, void*, size_t), size_t buffer_size) {
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
    receive_function(connfd, buffer, buffer_size);

    // Para o cronômetro
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Tempo decorrido: %.6f segundos\n", elapsed_time);
    printf("Throughput: %.2f MB/s\n", (buffer_size / 1024.0 / 1024.0) / elapsed_time);

    free(buffer);
}

int main() {
    int connfd = create_connection_listen(PORT);

    // Testar a função com buffer estático
    printf("Testando com buffer estático:\n");
    performance_test(connfd, receive_bytes_from_client_static_buff, TEST_DATA_SIZE);

    // Testar a função com buffer dinâmico
    printf("Testando com buffer dinâmico:\n");
    performance_test(connfd, receive_bytes_with_dynamic_buffer, TEST_DATA_SIZE);

    close_connection_server(connfd);
    return 0;
}
