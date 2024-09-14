#include <stdio.h>
#include "client.h"


/*
Author: Davi Souza de Luna
License: MIT
*/

/*
Referencia temporaria(para desenvolvimento):

typedef struct {
    char *client_id;       // Identificador do cliente (Client ID)
    uint16_t keep_alive;   // Tempo de keep-alive em segundos
    char *ip_broker;       // IP do broker
    uint16_t port_broker;  // Porta do broker
    uint8_t default_qos;   // Qualidade de Serviço padrão
}client_config_t;

typedef struct {
    int socket;          // Descriptor do socket para a conexão com o broker
    char *buffer;        // Buffer para receber e enviar dados
    size_t buffer_size;  // Tamanho do buffer
    client_config_t config;  // Configurações do cliente
}client_t;

typedef struct {
    char *topic;    // Tópico da mensagem
    char *payload;  // Dados da mensagem
    uint8_t qos;    // Qualidade de Serviço
    uint8_t retain; // Retain. Inicialmente não será utilizado. Portanto será 0. Quando for utilizar, altere para 1
}message_t;
*/


int create_client(client_t *client, client_config_t *config, size_t buffer_size){
    if (!client || !config) {
        return -1; // Verifica se os ponteiros são válidos
    }

    client->config.client_id = strdup(config->client_id); // Aloca memória para a string e copia o conteúdo, pois a string original pode ser liberada
    if(client->config.client_id == NULL){
        printf("Error allocating memory for client_id\n");
        return -1;
    }
    client->config.keep_alive = config->keep_alive;

    client->config.ip_broker = strdup(config->ip_broker); // Aloca memória para a string e copia o conteúdo, pois a string original pode ser liberada
    if(client->config.ip_broker == NULL){
        free(client->config.client_id);
        printf("Error allocating memory for ip_broker\n");
        return -1;
    }

    client->config.port_broker = config->port_broker;
    client->config.default_qos = config->default_qos;

    if(buffer_size == 0){
        buffer_size = INITIAL_BUFFER_SIZE; // Define o tamanho do buffer padrao
    }
    client->buffer_size = buffer_size; // Define o tamanho do buffer
    client->buffer = (char*)malloc(buffer_size); // Aloca memória para o buffer
    if(client->buffer == NULL){
        free(client->config.ip_broker);
        free(client->config.client_id);
        printf("Error allocating memory for buffer\n");
        return -1;
    }
    client->socket = -1; // Inicializa o socket com -1(ainda nao conectado)
    return 0;
};

/*

*/

int destroy_client(client_t *client){
//Os ifs verificam se a condicao realmente existe, para nao consumir tempo de processamento
    if(!client){
        return -1;
    }
    if(client->socket !=-1){
        close(client->socket);
        client->socket = -1; // Marca o socket como fechado atribuindo o valor -1 ao descritor
    }

    free(client->buffer);
    if (client->config.ip_broker) {
        free(client->config.ip_broker);
        client->config.ip_broker = NULL; // Marca ip_broker como liberado
    }
    if (client->config.client_id) {
        free(client->config.client_id);
        client->config.client_id = NULL; // Marca client_id como liberado
    }
    return 0;
}

int connect_client_TCP(client_t *client){
    if(!client){
        return -1;
    }
    int client_socket = create_connection_to_server(client->config.ip_broker, client->config.port_broker);
    if(client_socket < 0){
        fprintf(stderr, "Failed to connect to the server\n");
        return -1;
    }
    client->socket = client_socket;
    return 0;
};

int connect_client(client_t *client, protocol_t protocol){
    if (!client) {
        return -1; // Verifica se o ponteiro é válido
    }
    switch(protocol){
        case PROTOCOL_TCP:
            return connect_client_TCP(client);
        default:
            return -1; // Unknown protocol
    }
    return -1; //fail if the protocol is not defined
};

