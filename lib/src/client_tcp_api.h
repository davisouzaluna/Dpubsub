/*
Author: Davi Souza de Luna
License: MIT

This code has an API to connect to a MQTT broker. The functions are implemented in the client.c file.
*/
#ifndef CLIENT_TCP_API_H
#define CLIENT_TCP_API_H
#include <stdint.h>
#include <stdio.h>
#include "client.h"


/*
Connect to the broker using the client structure.
*/
int client_connect(client_t *client);


/*
Disconnect from the broker using the client structure. Free the resources.
*/
int client_disconnect(client_t *client);

/*
Sending the packet SUBSCRIBE to the broker. 
*/
int client_subscribe(client_t *client,const char *topic, uint16_t message_id);

/*
Sending the packet UNSUBSCRIBE to the broker.

TODO: Add the QoS parameter.
TODO: Add the client structure.
*/
int client_unsubscribe(client_t *client, const char *topic, uint16_t message_id);

/*
Sending a PUBLISH packet to an broker.
*/
int client_publish(client_t *client,const char *topic, char *message, uint16_t message_id, uint8_t retain, uint8_t dup);

/*
Check connection. Wait for the broker to send a PINGRESP packet.
*/
int client_pingreq(client_t *client);



int define_publish_cb(client_t *client, int (on_publish)(message_t *msg));

int on_publish(message_t *msg);

/*
called after subscribe ACK.
QoS 0 => ACK from Broker
QoS 1 => SUBACK from Broker
Is necessary to define the callback function.
*/
int define_subscribe_cb();


/**/
int default_subscribe_cb();

/*This function receive any message from the broker. It`s called if  */
int receive_message_sub_cb(client_t *client ,int (on_message)(message_t *msg));

/*It`s a default function called after receive a message after subscribe */
int on_message(message_t *msg);


/*
Handle the message received from the broker.
For example: PINGREQ, PINGRESP, SUBACK, UNSUBACK, PUBACK, PUBREC, PUBREL, PUBCOMP, CONNACK, DISCONNECT, CONNECT, PUBLISH.

IF the message is PUBLISH(after send SUBSCRIBE message to broker), the message is sent to the callback function.
*/
int handle_message(client_t *client);


#endif