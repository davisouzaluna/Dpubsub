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

/*
Function to create a client. It receives a client_t pointer, a client_config_t pointer and a buffer size. If the buffer size is not provided, it will use the default size defined 
in the macro INITIAL_BUFFER_SIZE.It returns 0 if the client is created successfully, -1 otherwise.
*/
int create_client(client_t *client, client_config_t *config, size_t buffer_size){
    if (!client || !config) {
        return -1; // Verifica se os ponteiros são válidos
    }

    client->config.client_id = config->client_id;
    if(client->config.client_id = NULL){
        return -1;
    }
    client->config.keep_alive = config->keep_alive;

    client->config.ip_broker = strdup(config->ip_broker); // Aloca memória para a string e copia o conteúdo, pois a string original pode ser liberada
    if(client->config.ip_broker = NULL){
        free(client->config.ip_broker);
        return -1;
    }

    client->config.port_broker = config->port_broker;
    client->config.default_qos = config->default_qos;

    if(buffer_size == NULL){
        buffer_size = INITIAL_BUFFER_SIZE; // Define o tamanho do buffer padrao
    }
    client->buffer_size = buffer_size; // Define o tamanho do buffer
    client->buffer = (char*)malloc(buffer_size); // Aloca memória para o buffer
    if(client->buffer = NULL){
        printf("Error allocating memory for buffer\n");
        return -1;
    }
    client->socket = -1; // Inicializa o socket com -1(ainda nao conectado)
    return 0;
};

/*

*/

int destroy_client(client_t *client){
    if(!client){
        return -1;
    }
    if(client->socket !=-1){
        close(client->socket);
    }

    free(client->buffer);
    free(client->config.ip_broker);
    free(client->config.client_id);
    return 0;
}