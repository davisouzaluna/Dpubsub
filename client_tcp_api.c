#include "client_tcp_api.h"


int client_connect_params(char *client_id, uint16_t keep_alive, char *ip_broker, uint16_t port_broker, uint8_t version){
    client_t *client = client_create();
    if(!client){
        return -1;
    }
    set_client_id(client, client_id);
    set_keep_alive(client, keep_alive);
    set_broker_ip(client, ip_broker);
    set_broker_port(client, port_broker);
    set_config(client, client_config_create(version));
    return connect_client(client, PROTOCOL_TCP);
};

int client_connect(client_t *client){
    return connect_client(client, PROTOCOL_TCP);
}

int client_disconnect_params(char *client_id, uint16_t keep_alive, char *ip_broker, uint16_t port_broker, uint8_t version){
    client_t *client = client_create();
    return disconnect_client(client, PROTOCOL_TCP);
}

int client_disconnect(client_t *client){
    return disconnect_client(client, PROTOCOL_TCP);
}

int client_subscribe(client_t *client,const char *topic, uint16_t message_id){
    
    if(subscribe_topic(client, topic, PROTOCOL_TCP)!=0){
        return -1;
        printf("Failed to subscribe to topic\n");
    }
    return 0;
};

