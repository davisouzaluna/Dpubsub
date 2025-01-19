#include "client_tcp_api.h"
#include "packets.h"

/*
This variable is used to control the keep alive time of the client.
*/
int g_keepalive;


int client_pingreq(client_t *client){
    char buffer[2];
    int create_packet = serialize_pingreq(PINGREQ, buffer, sizeof(buffer));
    if(create_packet!=0){
        return -1;
    }
    if(send_bytes_to_server(client->socket,buffer,sizeof(buffer))!=0){
        return -1;
    }
    return 0;
}


bool verify_value(int value1, int value2){
    if(value1 == value2){
        return true;
    }
    return false;
}

int check_keep_alive(client_t *client){
    if(verify_value(get_keep_alive(client), g_keepalive) == false){
        g_keepalive = get_keep_alive(client);
    }
    //TODO:Aqui tem que ter uma comparacao(difftime) pra verificar se o tempo "estourou". Sempre enviar um PINGREQ pelo menos quando estiver a 80 por cento 
    //do tempo(evitar desconexao por parte do broker)

    if(g_keepalive - get_keep_alive(client) >= g_keepalive-1){ // Faltando 1 segundo para o keepalive estourar
        client_pingreq(client);
    }

    return 0;
}


int client_connect(client_t *client){
    char buffer1[128];
    connect_client(client, PROTOCOL_TCP);

    if(verify_value(get_keep_alive(client), g_keepalive) == false){
        g_keepalive = get_keep_alive(client);
    }

    int serialize_CONNECT = serialize_connect(CONNECT, buffer1, sizeof(buffer1), get_client_id(client), get_keep_alive(client), 0x04);
    if(serialize_CONNECT < 0){
        return -1;
    }
    int send_connect_message = send_bytes_to_server(get_socket_fd(client), buffer1, serialize_CONNECT);
    if(send_connect_message != 0){
        return -1;
    }
    return 0;
}



int client_disconnect(client_t *client){

    char buffer[2];
    int disconnect = serialize_disconnect(DISCONNECT, buffer, sizeof(buffer));
    if(disconnect < 0){
        return -1;
    }
    int send_disconnect = send_bytes_to_server(get_socket_fd(client), buffer, disconnect);
    if(send_disconnect != 0){
        return -1;
    }
    disconnect_client(client, PROTOCOL_TCP);
    
    //liberar recursos
    destroy_client(client);
}

int client_subscribe(client_t *client,const char *topic, uint16_t message_id){
    if(verify_value(get_keep_alive(client), g_keepalive) == false){
        g_keepalive = get_keep_alive(client);
    }
    if(subscribe_topic(client, topic, PROTOCOL_TCP)!=0){
        return -1;
        printf("Failed to subscribe to topic\n");
    }
    return 0;
};

int client_unsubscribe(client_t *client, const char *topic, uint16_t message_id){
    int num_topics = 1; //aqui tbm deve ser implementada uma abstracao para o numero de topicos
    int topic_length = strlen(topic);
    int buffer_size_unsub = 2 + 2 + (2 + topic_length);

    char buffer[buffer_size_unsub];
    int unsubscribe = serialize_unsubscribe(UNSUBSCRIBE, buffer, sizeof(buffer), topic, message_id);
    if(unsubscribe < 0){
        return -1;
    }
    int send_unsubscribe = send_bytes_to_server(get_socket_fd(client), buffer, unsubscribe);
    if(send_unsubscribe != 0){
        return -1;
    }
    return 0;
}

int client_publish(client_t *client, const char *topic, char *message, uint16_t message_id, uint8_t retain, uint8_t dup){
    if(verify_value(get_keep_alive(client), g_keepalive) == false){
        g_keepalive = get_keep_alive(client);
    }
    if(publish(client, topic, message, message_id, retain, dup)!=0){
        printf("Failed to publish message\n");
        return -1;
    }
    return 0;
}


int define_publish_cb(client_t *client, int (on_publish)(message_t *msg)){
    
};
