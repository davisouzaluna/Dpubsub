#include "client_tcp_api.h"
#include "packets.h"


int client_connect_params(char *client_id, uint16_t keep_alive, char *ip_broker, uint16_t port_broker, uint8_t version){
    client_t client;
    client_config_t config;
    
    /*
    set_client_id(&client, client_id);
    set_keep_alive(&client, keep_alive);
    set_broker_ip(&client, ip_broker);
    set_broker_port(&client, port_broker);
    set_config(&client, &config);
    create_client(&client, &config, 0);
    */
    config.client_id = client_id;
    config.keep_alive = keep_alive;
    config.ip_broker = ip_broker;
    config.port_broker = port_broker;
    config.default_qos = 0;
    create_client(&client, &config, 0);


    return connect_client(&client, PROTOCOL_TCP);
};

int client_connect(client_t *client){
    char buffer1[128];
    connect_client(client, PROTOCOL_TCP);

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

int client_disconnect_params(char *client_id, uint16_t keep_alive, char *ip_broker, uint16_t port_broker, uint8_t version){
    client_t *client;
    client_config_t *config;
    set_client_id(client, client_id);
    set_keep_alive(client, keep_alive);
    set_broker_ip(client, ip_broker);
    set_broker_port(client, port_broker);
    set_config(client, config);
    create_client(client, config, 0);
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

int client_publish(client_t *client, const char *topic, const char *message, uint16_t message_id, uint8_t retain, uint8_t dup){
    if(publish(client, topic, message, message_id, retain, dup)!=0){
        printf("Failed to publish message\n");
        return -1;
    }
    return 0;
}

int client_pingreq(client_t *client){
    char buffer[2];
    int create_packet = serialize_pingreq(PINGREQ, buffer, sizeof(buffer));
    if(create_packet!=0){
        return -1;
    }
    if(send_message(client, buffer, PROTOCOL_TCP)!=0){
        return -1;
    }
    return 0;
}
