#include "dpubsub.h"


/*
TODO: Only tcp here. After implement other protocols create other methods, like: dbupsub_config_quic() or dpubsub_config_udp()
*/

int dpubsub_config(client_t *client, client_config_t *config, size_t buffer_size, dpubsub_protocol_t protocol){
    int cliente;
    if(protocol == PROTOCOL_TCP){
        cliente = create_client(client,config,buffer_size);
    }
    
    return cliente;
};

int dpubsub_define_client_struct(client_t client, client_config_t config, size_t buffer_size){
    int cliente;
    client.config = config;
    client.buffer_size = buffer_size;
    return cliente;

}
int dpubsub_define_config_struct(client_config_t *config, char *client_id, uint16_t keep_alive, char *ip_broker, uint16_t port_broker, uint8_t default_qos){
    
    if(!config){
        return -1;
    }
    config->client_id = client_id;
    config->keep_alive = keep_alive;
    config->ip_broker = ip_broker;
    config->port_broker = port_broker;
    config->default_qos = default_qos;
    
    return 0;
};


int dpubsub_connect(client_t *client, dpubsub_protocol_t protocol){
    int connect;

    if(protocol == PROTOCOL_TCP){
        connect = client_connect(client);
    }
    return connect;
};

int dpubsub_disconnect(client_t *client, dpubsub_protocol_t protocol){
    int disconnect;
    if(protocol == PROTOCOL_TCP){
        disconnect = client_disconnect(client);
    }
    return disconnect;
};

int dpubsub_subscribe(client_t *client,const char *topic, uint16_t message_id, dpubsub_protocol_t protocol){
    int subscribe;
    subscribe = client_subscribe(client,topic,message_id);
    return subscribe;
};

int dpubsub_unsubscribe(client_t *client, const char *topic, uint16_t message_id, dpubsub_protocol_t protocol){
    int unsubscribe;
    unsubscribe = client_unsubscribe(client,topic,message_id);
    return unsubscribe;
};

int dpubsub_publish(client_t *client,const char *topic, char *message, uint16_t message_id, uint8_t retain, uint8_t dup, dpubsub_protocol_t protocol){
    int publish;
    publish = client_publish(client,topic,message,message_id,retain,dup);
    return publish;
};

int dpubsub_pingreq(client_t *client, dpubsub_protocol_t protocol){
    int pingreq;

    if (protocol == PROTOCOL_TCP){
    pingreq = client_pingreq(client);
    }
    return pingreq;
};
