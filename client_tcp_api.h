/*
Author: Davi Souza de Luna
License: MIT

This code has an API to connect to a MQTT broker. The functions are implemented in the client.c file.
*/

#include <stdint.h>

/*
Connect to the broker. 
*/
int client_connect(char *client_id, uint16_t keep_alive, char *ip_broker, uint16_t port_broker, uint8_t version);

/*
Disconnect from the broker. Free the resources.
*/
int client_disconnect();

/*
Sending the packet SUBSCRIBE to the broker. 
*/
int client_subscribe(const char *topic, uint16_t message_id, uint8_t qos);

/*
Sending the packet UNSUBSCRIBE to the broker.
*/
int client_unsubscribe(const char *topic, uint16_t message_id);

/*
Sending a PUBLISH packet to an broker.
*/
int client_publish(const char *topic, const char *message, uint16_t message_id, uint8_t qos, uint8_t retain, uint8_t dup);

/*
Check connection
*/
int client_pingreq();

