#ifndef CLIENT_H
#define CLIENT_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "transport/tcp/tcp_client.h"

#define INITIAL_BUFFER_SIZE 1024 // Tamanho inicial do buffer de recebimento

/*

//configurações do cliente. Aqui contem as configurações pertinentes para a conexao a nivel de aplicação
struct client_config{
    id;
    tempo de conexao;
    keep_alive;
    ip_broker;
    port_broker;
    default_qos;
}

//estrutura do cliente. Aqui contem as configuracoes pertinentes para a conexao a nivel de transporte
struct client{
    socket;
    buffer;
    buffer_size;

    client_config config; // struct client_config
}

//estrutura da mensagem. Aqui contem as informacoes da mensagem a ser enviada
struct message{
    topic;
    payload;
    qos;
    retain;
}


// Funções básicas para uma comunicação 
int create_client(struct client *client, struct client_config *config);
int destroy_client(struct client *client); // Fecha o socket e libera a memória 
int connect_client(struct client *client); // Conecta o cliente ao broker
int disconnect_client(struct client *client); // Desconecta o cliente do broker
int send_message(struct client *client, struct message *msg); // Envia uma mensagem ao broker. O cliente deve estar conectado. Aqui seria feito a serialização da mensagem e enviada a nível de transporte 
int receive_message(struct client *client, struct message *msg); // Recebe uma mensagem do broker. O cliente deve estar conectado. Aqui seria feito a desserialização da mensagem recebida a nível de transporte(um subscribe por exemplo).
int subscribe_topic(struct client *client, char *topic); // Inscreve o cliente em um tópico
int unsubscribe_topic(struct client *client, char *topic); // Cancela a inscrição do cliente em um tópico
int keep_alive(struct client *client); // Envia um PINGREQ ao broker. Seria pra indicar que o cliente ainda está ativo. O broker deve responder com PINGRESP

//getters e setters

int set_qos(struct client *client, int qos); // Define o QoS padrão do cliente
int get_qos(struct client *client, int *qos); // Obtém o QoS padrão do cliente

int set_keep_alive(struct client *client, int keep_alive); // Define o tempo de keep alive do cliente
int get_keep_alive(client_t *client, uint16_t *keep_alive); // Obtém o tempo de keep alive do cliente

int set_broker(struct client *client, char *ip, int port); // Define o endereço do broker
int get_broker(struct *client, char *ip, int *port); // Obtém o endereço do broker

int set_socket(struct client *client, int socket); // Define o socket do cliente
int get_socket(struct client *client, int *socket); // Obtém o socket do cliente

int set_id(struct client *client, char *id); // Define o ID do cliente
int get_id(struct client *client, char *id); // Obtém o ID do cliente

int set_buffer_size(struct client *client, size_t buffer_size); // Define o tamanho do buffer
int get_buffer_size(struct client *client, size_t *buffer_size); // Obtém o tamanho do buffer

int set_buffer(struct client *client, void *buffer); // Define o buffer
int get_buffer(struct client *client, void *buffer); // Obtém o buffer

int set_config(struct client *client, struct client_config *config); // Define a configuração do cliente
int get_config(struct client *client, struct client_config *config); // Obtém a configuração do cliente





*/

/*
Enum to define the protocol used to connect to the broker
*/
typedef enum {
    PROTOCOL_TCP
} protocol_t;

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

/*
Function to create a client. It receives a client_t pointer, a client_config_t pointer and a buffer size. If the buffer size is not provided, it will use the default size defined 
in the macro INITIAL_BUFFER_SIZE.It returns 0 if the client is created successfully, -1 otherwise.
*/
int create_client(client_t *client, client_config_t *config, size_t buffer_size);

// Destroy a client, free memory and close socket
int destroy_client(client_t *client);

/* Connect to the broker using TCP. The client must be created before connecting. Returns 0 if the connection is successful, -1 otherwise.
The TCP connection is made with the create_connection_to_server function, which receives the broker's IP and port. She is
located in the file /transport/tcp/tcp_client.h
*/
int connect_client_TCP(client_t *client);

/* Connect to the broker. The client must be created before connecting. The protocol is defined in enum protocol_t. It returns 0 if the connection is successful, -1 otherwise.
*/
int connect_client(client_t *client, protocol_t protocol);

/*
Disconnect from the broker using TCP protocol. The client must be connected before disconnecting. Returns 0 if the disconnection is successful, -1 otherwise.
*/
int disconnect_client_TCP(client_t *client);

// Disconnect from the broker
int disconnect_client(client_t *client, protocol_t protocol);

// Send a message to the broker using TCP.
int send_message_TCP(client_t *client, message_t *msg);

// Send a message to the broker. The client must be connected before sending the message. It returns 0 if the message is sent successfully, -1 otherwise.
int send_message(client_t *client, message_t *msg, protocol_t protocol);

// Free the message. It is necessary to free the memory allocated for the message
int free_message(message_t *msg);

// Receive a message from the broker using TCP
int receive_message_TCP(client_t *client);

// Receive a message from the broker
int receive_message(client_t *client, protocol_t protocol);

// Subscribe to a topic
int subscribe_topic(client_t *client, char *topic);


// Unsubscribe from a topic
int unsubscribe_topic(client_t *client, char *topic);

// Send a PINGREQ to the broker. The broker should respond with PINGRESP
int keep_alive(client_t *client);


// Getters and Setters
/*
Getters and Setters functions for the client structure
*/
// Set the QoS of the client
int set_qos(client_t *client, uint8_t qos);

// Get the QoS of the client
int get_qos(client_t *client, uint8_t *qos);

// Set the keep-alive time of the client
int set_keep_alive(client_t *client, uint16_t keep_alive);

// Get the keep-alive time of the client
int get_keep_alive(client_t *client, uint16_t *keep_alive);

// Set the broker IP and port
int set_broker(client_t *client , char *ip, uint16_t port);

// Get the broker IP and port
int get_broker(client_t *client, char *ip, uint16_t *port);

// Set the socket of the client
int set_socket(client_t *client, int socket);

// Get the socket of the client 
int get_socket(client_t *client, int *socket);

// Set the ID of the client
int set_client_id(client_t *client, char *client_id);

// Get the ID of the client
int get_client_id(client_t *client, char *client_id);

// Set the buffer size
int set_buffer_size(client_t *client, size_t buffer_size);

// Get the buffer size
int get_buffer_size(client_t *client, size_t *buffer_size);

// Set the buffer
int set_buffer(client_t *client, void *buffer);

// Get the buffer
int get_buffer(client_t *client, void *buffer);

// Set the client configuration
int set_config(client_t *client, client_config_t *config);

// Get the client configuration
int get_config(client_t *client, client_config_t *config);

#endif // CLIENT_H