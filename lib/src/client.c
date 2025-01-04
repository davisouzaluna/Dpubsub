#include <stdio.h>
#include "client.h"
#include "packets.h"


/*
Author: Davi Souza de Luna
License: MIT


*/

//==================================================Callbacks=========================================================

//define default callback functions
int on_publish(message_t *msg){
    if(!msg){
        return -1;
    }
    char *topic = (char*)msg->topic;
    char *payload = msg->payload;
    printf("Message published in topic '%s': %s\n", topic, payload);
    return 0;
};
int on_subscribe(message_t *msg){
    if(!msg){
        return -1;
    }
    char *topic = (char*)msg->topic;
    printf("Subscribed to topic '%s'\n", topic);
    return 0;
};
int on_disconnect(message_t *msg){
    if(!msg){
        return -1;
    }
    printf("Disconnected from broker\n");
    return 0;
};
int on_connect(message_t *msg){
    if(!msg){
        return -1;
    }
    printf("Connected to broker\n");
    return 0;
};
int on_unsubscribe(message_t *msg){
    if(!msg){
        return -1;
    }
    char *topic = (char*)msg->topic;
    printf("Unsubscribed from topic '%s'\n", topic);
    return 0;
};
int on_message(message_t *msg){
    if(!msg){
        return -1;
    }
    char *topic = (char*)msg->topic;
    char *payload = msg->payload;
    printf("Message received in topic '%s': %s\n", topic, payload);
    return 0;
};

int define_default_callback(client_t *client){
    if(!client){
        return -1;
    }
    
    client->config.callbacks.on_subscribe = on_subscribe;
    client->config.callbacks.on_publish = on_publish;
    client->config.callbacks.on_disconnect = on_disconnect;
    client->config.callbacks.on_connect = on_connect;
    client->config.callbacks.on_unsubscribe = on_unsubscribe;
    client->config.callbacks.on_message =  on_message;
    return 0;
}

int define_null_cb(client_t *client){
    if(!client){
        return -1;
    }
    client->config.callbacks.on_subscribe = NULL;
    client->config.callbacks.on_publish = NULL;
    client->config.callbacks.on_disconnect = NULL;
    client->config.callbacks.on_connect = NULL;
    client->config.callbacks.on_unsubscribe = NULL;
    client->config.callbacks.on_message =  NULL;
    return 0;
}

//====================================================================================================================



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

    if(define_default_callback(client)!=0){ //define os callbacks padrao
        free(client->config.ip_broker);
        free(client->config.client_id);
        free(client->buffer);
        printf("Error defining default callback\n");
        return -1;
    }
    
    if(config)
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

int disconnect_client_TCP(client_t *client){
    if(!client){
        return -1;
    }
    if (client->socket < 0) {
        return -1; // Verifica se o socket está conectado
        printf("socket ja desconectado\n");
    }
    int close_connection = close_connection_client(client->socket);
    if(close_connection < 0){
        fprintf(stderr, "Failed to disconnect client\n");
        return -1;
    }

    //callback
    if(client->config.callbacks.on_disconnect){
        message_t *msg;
        client->config.callbacks.on_disconnect(msg);
    }
};

int disconnect_client(client_t *client, protocol_t protocol){
    if (!client) {
        return -1; // Verifica se o ponteiro é válido
    }
    switch(protocol){
        case PROTOCOL_TCP:
            return disconnect_client_TCP(client);
        default:
            return -1; // Unknown protocol
    }
};

int send_message_TCP(client_t *client, message_t *msg){
    if(!client || !msg){
        return -1;
    }
    if(client->socket < 0){
        return -1;
    }

    //Por enquanto ele somente envia a struct message_t
    int bytes_sent = send_bytes_to_server(client->socket, msg, sizeof(msg));
    if(bytes_sent < 0){
        fprintf(stderr, "Failed to send message\n");
        return -1;
    }
    return 0;
};

//TODO: Mudar a chamada do switch case(send_message_TCP) para 
int send_message(client_t *client, message_t *msg, protocol_t protocol){
    if(!client || !msg){
        return -1;
    }
    switch(protocol){
        case PROTOCOL_TCP:
            return send_message_TCP(client, msg);
        default:
            return -1; // Unknown protocol
    }
}

int free_message(message_t *msg){
    if(!msg){
        return -1;
    }
    if(msg->topic){
        free((char*)msg->topic);
        msg->topic = NULL;
    }
    if(msg->payload){
        free(msg->payload);
        msg->payload = NULL;
    }
    return 0;
}

int receive_message_TCP(client_t *client){
    if(!client){
        return -1;
    }
    if(client->socket < 0){
        return -1;
    }

    //caso o buffer nao tenha sido alocado(primeiro vem o tamanho do buffer)
    if(client->buffer_size == 0){
        client->buffer_size = INITIAL_BUFFER_SIZE; // Define o tamanho do buffer padrao
    }
    //caso o buffer nao esteja alocado
    if(client->buffer == NULL){
        client->buffer = (char*)malloc(sizeof(char)*client->buffer_size);
        if(client->buffer == NULL){
            fprintf(stderr, "Failed to allocate memory for buffer\n");
            return -1;
        }
    }
   
    int bytes_received = receive_bytes_from_server_static_buff(client->socket, client->buffer, client->buffer_size);
    if(bytes_received < 0){
        fprintf(stderr, "Failed to receive message\n");
        return -1;
    }
    return bytes_received;
}

int receive_message(client_t *client, protocol_t protocol){
    if(!client){
        return -1;
    }
    if(client->socket < 0){
        return -1;
    }
    switch(protocol){
        case PROTOCOL_TCP:
            return receive_message_TCP(client);
        default:
            return -1; // Unknown protocol
    }
}

int subscribe_topic(client_t *client, const char *topic, protocol_t protocol)
{
    // char buffer = (char*)malloc(256);//Criar inicialmente um buffer diferente do cliente(posteriormente terei que alocar e desalocar o buffer da estrutura do cliente)
    if (protocol = PROTOCOL_TCP)
    {
        char buffer[256];
        if (buffer == NULL)
        {
            return -1;
        }
        int message_id = 1;        // Gerar um id de mensagem
        int qos = get_qos(client); // Qualidade de serviço
        int create_packet = serialize_subscribe(SUBSCRIBE, buffer, sizeof(buffer), topic, message_id, qos);
        if (create_packet < 0)
        {
            // free(buffer);
            return -1;
        }

        if (send_bytes_to_server(get_socket_fd(client), buffer, create_packet) != 0)
        {
            fprintf(stderr, "Failed to send SUBSCRIBE packet\n");
            return -1;
        }

        // callback
        uint8_t retain = 0;
        message_t *msg;
        msg->topic = topic;

        msg->qos = qos;
        // printf("QoS: %d\n", msg->qos);
        msg->retain = retain;

        if (client->config.callbacks.on_subscribe)
        {
            client->config.callbacks.on_subscribe(msg);
        }

        // free(buffer);
        return 0;
    }
};

int publish(client_t *client, const char *topic,  char *message, uint16_t message_id, uint8_t retain, uint8_t dup){
    //char buffer = (char*)malloc(256);//Criar inicialmente um buffer diferente do cliente(posteriormente terei que alocar e desalocar o buffer da estrutura do cliente)
    char buffer3[256];
    /*
    if(buffer == NULL){
        return -1;
    }
    */
    message_t *msg;
    msg->topic = topic;
    msg->payload = message;
    msg->qos = get_qos(client);
    printf("QoS: %d\n", msg->qos);
    msg->retain = retain;
    int create_packet = serialize_publish(PUBLISH, buffer3, sizeof(buffer3), msg->topic, msg->payload, message_id, msg->qos, retain, dup);
    /*
    if(create_packet < 0){
        free(buffer);
        return -1;
    }
    */
    if(send_bytes_to_server(get_socket_fd(client), buffer3, create_packet)!=0){
        fprintf(stderr, "Failed to send message and PUBLISH packet\n");
        return -1;
    }

    /*
    if(send_packet < 0){
        free(buffer);
        return -1;
    }
    free(buffer);
    */

   //callback
   if(client->config.callbacks.on_publish){
       client->config.callbacks.on_publish(msg);
   }
    return 0;
}



//============================================Getters and Setters============================================

int set_qos(client_t *client, uint8_t qos){
    if(!client){
        return -1;
    }
    client->config.default_qos = qos;
    return 0;
}

int get_qos(client_t *client){
    if(!client){
        return -1;
    }
    return client->config.default_qos;
}

int set_keep_alive(client_t *client, uint16_t keep_alive){
    if(!client){
        return -1;
    }
    client->config.keep_alive = keep_alive;
    return 0;
}

int get_keep_alive(client_t *client){
    if(!client){
        return -1;
    }
    return client->config.keep_alive;
}

int set_broker_ip(client_t *client , char *ip){
    if(!client){
        return -1;
    }
    if(client->config.ip_broker){
        free(client->config.ip_broker);
    }
    client->config.ip_broker = strdup(ip);
    if(client->config.ip_broker == NULL){
        return -1; // Error allocating memory
    }
    return 0;
}

char* get_broker_ip(client_t *client){
    if(!client){
        return NULL;
    }
    return client->config.ip_broker;
}

int set_broker_port(client_t *client, uint16_t port){
    if(!client){
        return -1;
    }
    client->config.port_broker = port;
    return 0;
}

int get_broker_port(client_t *client){
    if(!client){
        return -1;
    }
    return client->config.port_broker;
}

int set_socket_fd(client_t *client, int socket){
    if(!client){
        return -1;
    }
    client->socket = socket;
    return 0;
}

int get_socket_fd(client_t *client){
    if(!client){
        return -1;
    }
    return client->socket;
}

int set_client_id(client_t *client, char *client_id){
    if(!client){
        return -1;
    }
    if(client->config.client_id){
        free(client->config.client_id);
    }
    client->config.client_id = strdup(client_id);
    if(client->config.client_id ==NULL){
        return -1; // Error allocating memory
    }
    return 0;
}

char* get_client_id(client_t *client){
    if(!client){
        return NULL;
    }
    return client->config.client_id;
}

int set_buffer_size(client_t *client, size_t buffer_size){
    if(!client){
        return -1;
    }
    client->buffer_size = buffer_size;
    return 0;
}

size_t get_buffer_size(client_t *client){
    if(!client){
        return -1;
    }
    return client->buffer_size;
}

//TODO: Rever a funcao set_buffer

/*
Ideia utilizando o tamanho do buffer como parametro:
int set_buffer(client_t *client, void *buffer, size_t buffer_size){
    if(!client){
        return -1;
    }
    if(client->buffer){
        free(client->buffer);
    }
    client->buffer = malloc(buffer_size);
    if(client->buffer == NULL){
        return -1;
    }
    memcpy(client->buffer, buffer, buffer_size); // Copia os dados
    client->buffer_size = buffer_size;
    return 0;
}


*/
int set_buffer(client_t *client, void *buffer){
    if(!client){
        return -1;
    }
    if(client->buffer){
        free(client->buffer);
    }
    client->buffer = (char*)buffer;
    return 0;
};

char* get_buffer(client_t *client){
    if(!client){
        return NULL;
    }
    return client->buffer;
}

int set_config(client_t *client, client_config_t *config){
    if(!client || !config){
        return -1;
    }
    if(client->config.client_id){
        free(client->config.client_id);
    }
    if(client->config.ip_broker){
        free(client->config.ip_broker);
    }
    if(config->client_id){
        client->config.client_id = strdup(config->client_id);
        if(!client->config.client_id){
            return -1;
        }

    }else{
        client->config.client_id = NULL; // Se ele for nulo, deixa NULL
    }

    
    if (config->ip_broker) {
        client->config.ip_broker = strdup(config->ip_broker);
        if (!client->config.ip_broker) {
            free(client->config.client_id);
            return -1;  
        }
    } else {
        client->config.ip_broker = NULL; // Se ele for nulo, deixa NULL
    }

    
    client->config.keep_alive = config->keep_alive;
    client->config.port_broker = config->port_broker;
    client->config.default_qos = config->default_qos;

    return 0;  // Success
}

client_config_t get_config(client_t *client){
    client_config_t empty_config = {0}; // Return an empty config if client is NULL
    if(!client){
        return empty_config;
    }
    return client->config;
};



