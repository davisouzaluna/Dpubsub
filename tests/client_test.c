#include <stdio.h>
#include <stdlib.h>
#include "client.h"

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

    // Testar o envio de dados para o broker
    char *msg = "Hello, server!";
    message_t message;
    message.topic = "test_topic";
    message.payload = msg;
    message.qos = 1;
    message.retain = 0;
    if (send_message(&client, &message, PROTOCOL_TCP) != 0) {
        fprintf(stderr, "Failed to send message\n");
        disconnect_client(&client, PROTOCOL_TCP); // Certifique-se de desconectar o cliente em caso de falha no envio da mensagem
        destroy_client(&client); // Certifique-se de liberar os recursos em caso de falha no envio da mensagem
        return EXIT_FAILURE;
    }
    
    int received_message = receive_message(&client, PROTOCOL_TCP);
    //testar o recebimento de dados do broker
    if (received_message<0) {
        fprintf(stderr, "Failed to receive message\n");
        disconnect_client(&client, PROTOCOL_TCP); // Certifique-se de desconectar o cliente em caso de falha no recebimento da mensagem
        destroy_client(&client); // Certifique-se de liberar os recursos em caso de falha no recebimento da mensagem
        return EXIT_FAILURE;
    }
    message_t received;
    

    // Testar a desconexão do broker usando TCP
    if (disconnect_client(&client, PROTOCOL_TCP) != 0) {
        fprintf(stderr, "Failed to disconnect client\n");
        destroy_client(&client); // Certifique-se de liberar os recursos em caso de falha na desconexão
        return EXIT_FAILURE;
    }

    if(free_message(&received)!=0){
        fprintf(stderr, "Failed to free message\n");
        return EXIT_FAILURE;
    }else{
        printf("Message freed successfully\n");
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
