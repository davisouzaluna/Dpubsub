/*
Author: Davi Souza de Luna
License: MIT
About: dpubsub header file. Contains the functions to configure, connect, disconnect, subscribe, unsubscribe and publish messages to a broker.
*/

#ifndef DPUBSUB_H
#define DPUBSUB_H

#include "../client.h"
#include "../packets.h"
#include "../client_tcp_api.h" // TCP API


/*
This Enum will contain the protocols that will be used to connect to the broker. 
This enum is used by all functions that need to perform some type of transport layer service

TODO: If I want to use another protocol, I need to create another function like dpubsub_config_quic() or dpubsub_config_udp()
TODO: improve the description of this enum

*/
typedef enum {
    DPUBSUB_TCP
} dpubsub_protocol_t;



/*
TODO: If I want to use another protocol, I need to create another function like dpubsub_config_quic() or dpubsub_config_udp() 
*/

/*
Set TCP client configuration.
*/
int dpubsub_config(client_t *client, client_config_t *config, size_t buffer_size, dpubsub_protocol_t protocol);

int dpubsub_define_config_struct(client_config_t *config, char *client_id, uint16_t keep_alive, char *ip_broker, uint16_t port_broker, uint8_t default_qos);

int dpubsub_define_client_struct(client_t client, client_config_t config, size_t buffer_size);

int dpubsub_connect(client_t *client, dpubsub_protocol_t protocol);

int dpubsub_disconnect(client_t *client, dpubsub_protocol_t protocol);

int dpubsub_subscribe(client_t *client,const char *topic, uint16_t message_id, dpubsub_protocol_t protocol);

int dpubsub_unsubscribe(client_t *client, const char *topic, uint16_t message_id, dpubsub_protocol_t protocol);

int dpubsub_publish(client_t *client,const char *topic, char *message, uint16_t message_id, uint8_t retain, uint8_t dup, dpubsub_protocol_t protocol);

int dpubsub_pingreq(client_t *client, dpubsub_protocol_t protocol);


#endif // DPUBSUB_H