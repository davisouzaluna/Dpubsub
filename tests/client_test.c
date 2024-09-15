#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include "../../transport/tcp/tcp_client.h"

int main() {
    client_t client;
    client_config_t config;
    size_t buffer_size = 0; // Tamanho do buffer será definido pelo padrão se for 0

    // Configuração do cliente
    config.client_id = "test_client";
    config.keep_alive = 60;
    config.ip_broker = "127.0.0.1"; // IP de teste
    config.port_broker = 9090; // Porta do servidor de teste: ../transport/tcp/tests/server.c
    config.default_qos = 1;

    // Testar a criação do cliente
    if (create_client(&client, &config, buffer_size) != 0) {
        fprintf(stderr, "Failed to create client\n");
        return EXIT_FAILURE;
    }
    printf("Client created successfully\n");

    // Testar a conexão com o broker usando TCP
    if (connect_client(&client, PROTOCOL_TCP) != 0) {
        fprintf(stderr, "Failed to connect client\n");
        destroy_client(&client); // Certifique-se de liberar os recursos em caso de falha na conexão
        return EXIT_FAILURE;
    }
    printf("Client connected successfully\n");

    // Testar a desconexão do broker usando TCP
    if (disconnect_client(&client, PROTOCOL_TCP) != 0) {
        fprintf(stderr, "Failed to disconnect client\n");
        destroy_client(&client); // Certifique-se de liberar os recursos em caso de falha na desconexão
        return EXIT_FAILURE;
    }
    printf("Client disconnected successfully\n");
    // Testar a destruição do cliente
    if (destroy_client(&client) != 0) {
        fprintf(stderr, "Failed to destroy client\n");
        return EXIT_FAILURE;
    }
    printf("Client destroyed successfully\n");

    return EXIT_SUCCESS;
}
