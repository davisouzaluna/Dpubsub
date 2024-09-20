// tests/api_test.c
#include <stdio.h>
#include "client_tcp_api.h"

// Variáveis globais para IP e porta do broker
char *global_ip_broker = "91.121.93.94";
uint16_t global_port_broker = 1883;

// Função para testar a conexão do cliente com parâmetros individuais
void test_client_connect_params() {
    char *client_id = "test_client";
    uint16_t keep_alive = 60;
    uint8_t version = 4;

    int result = client_connect_params(client_id, keep_alive, global_ip_broker, global_port_broker, version);
    if (result == 0) {
        printf("Client connected successfully using parameters!\n");
    } else {
        printf("Failed to connect client using parameters.\n");
    }
}

// Função para testar a conexão do cliente com uma estrutura `client_t`
void test_client_connect() {
    client_t client;
    client_config_t config;

    set_client_id(&client, "test_client_struct");
    set_keep_alive(&client, 60);
    set_broker_ip(&client, global_ip_broker);
    set_broker_port(&client, global_port_broker);
    set_config(&client, &config);

    create_client(&client, &config, 0);

    int result = client_connect(&client);
    if (result == 0) {
        printf("Client connected successfully using structure!\n");
    } else {
        printf("Failed to connect client using structure.\n");
    }
}

// Função para testar a publicação de uma mensagem
void test_client_publish() {
    client_t client;
    client_config_t config;

    set_client_id(&client, "test_publish");
    set_keep_alive(&client, 60);
    set_broker_ip(&client, global_ip_broker);
    set_broker_port(&client, global_port_broker);
    set_config(&client, &config);

    create_client(&client, &config, 0);
    client_connect(&client);

    int result = client_publish(&client, "test/topic", "Hello, World!", 1, 0, 0);
    if (result == 0) {
        printf("Message published successfully!\n");
    } else {
        printf("Failed to publish message.\n");
    }

    client_disconnect(&client);
}

// Função para testar a assinatura em um tópico
void test_client_subscribe() {
    client_t client;
    client_config_t config;

    set_client_id(&client, "test_subscribe");
    set_keep_alive(&client, 60);
    set_broker_ip(&client, global_ip_broker);
    set_broker_port(&client, global_port_broker);
    set_config(&client, &config);

    create_client(&client, &config, 0);
    client_connect(&client);

    int result = client_subscribe(&client, "test/topic", 1);
    if (result == 0) {
        printf("Subscribed to topic successfully!\n");
    } else {
        printf("Failed to subscribe to topic.\n");
    }

    client_disconnect(&client);
}

// Função principal para rodar os testes
int main() {
    printf("Testing client connect with parameters...\n");
    test_client_connect_params();

    printf("\nTesting client connect with structure...\n");
    test_client_connect();

    printf("\nTesting client publish...\n");
    test_client_publish();

    printf("\nTesting client subscribe...\n");
    test_client_subscribe();

    return 0;
}
