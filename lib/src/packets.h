#ifndef PACKETS_H
#define PACKETS_H

#include <stddef.h>
#include <stdint.h>
#include "client.h"
/*
Nesse codigo, e onde as mensagems irao ser serializadas e desserializadas. tambem ira ser colocado um bit identificador em cada pacote para identificar de que tipo ele eh(analogo a uma flag)
A titulo de informacao, no comeco de toda mensagem devera ser colocado um byte de controle, que ira indicar o tipo da mensagem. entao quando for feita a desserializacao, o primeiro byte sera lido e sera identificado o tipo da mensagem
Isso tudo se encontra na RFC


tabela contendo o grupo de pacotes:

Pacote	    Código	Descrição
CONNECT	    0x01	Inicia uma conexão com o broker.
CONNACK	    0x00	Resposta ao pacote CONNECT; indica se a conexão foi aceita ou se houve um erro.
PUBLISH	    0x03	Publica uma mensagem em um tópico.
PUBACK	    0x04	Confirma o recebimento de uma mensagem com QoS 1.
PUBREC	    0x05	Confirma o recebimento de uma mensagem com QoS 2 (primeiro passo do QoS 2).
PUBREL	    0x06	Confirma que o broker está pronto para publicar a mensagem com QoS 2 (segundo passo do QoS 2).
PUBCOMP	    0x07	Confirma que a mensagem com QoS 2 foi completamente recebida e processada (terceiro passo do QoS 2).
SUBSCRIBE	0x08	Solicita inscrição em um ou mais tópicos.
SUBACK	    0x09	Confirma o recebimento do pacote SUBSCRIBE; indica o QoS concedido para cada tópico.
UNSUBSCRIBE	0x0A	Solicita o cancelamento da inscrição em um ou mais tópicos.
UNSUBACK	0x0B	Confirma o recebimento do pacote UNSUBSCRIBE.
PINGREQ	    0x0C	Solicita uma resposta do broker para verificar se ele ainda está conectado e funcionando.
PINGRESP	0x0D	Responde ao pacote PINGREQ; indica que o broker está conectado e funcionando.
DISCONNECT	0x0E	Solicita a desconexão do cliente do broker.


devera existir um enum que ira conter todos os pacotes e os bits identificadores de cada um

portanto cada pacote devera ter a funcao:

serialize_<pacote>(); //adiciona o bit idewntificador no pacote

deserialize(); //remove o bit identificador do pacote e identifica qual pacote ele eh e toma as acoes pertinentes


*/

/*
Enum if contain packet types and their respective codes
*/
typedef enum {
    CONNECT     = 0x01,
    CONNACK     = 0x02,
    PUBLISH     = 0x03,
    PUBACK      = 0x04,
    PUBREC      = 0x05,
    PUBREL      = 0x06,
    PUBCOMP     = 0x07,
    SUBSCRIBE   = 0x08,
    SUBACK      = 0x09,
    UNSUBSCRIBE = 0x0A,
    UNSUBACK    = 0x0B,
    PINGREQ     = 0xC0,
    PINGRESP    = 0x0D,
    DISCONNECT  = 0xE0
} packet_type_code_t;

/*
Functions to serialize packets. The packets are serialized in the buffer argument. The buffer_size argument is the size of the buffer.
If the serialize process is sucessful, the function returns 0. Otherwise, it returns -1.
*/ 

// Functions to serialize and deserialize packets
int switch_functions(packet_type_code_t packet_type, char *buffer, size_t buffer_size);

// Function to serialize connect packets(client->broker). 
int serialize_connect(packet_type_code_t packet_type, char *buffer, size_t buffer_size, const char *client_id, uint16_t keep_alive, uint8_t version);

// Function to deserialize connect packets(broker->client)
int serialize_connack(packet_type_code_t packet_type, char *buffer, size_t buffer_size, uint8_t session_present, uint8_t return_code);

// Function to encode the remaining length of the packet
int encode_remaining_length(size_t length, char *buffer, size_t buffer_size);

/* Function to serialize publish packets(client->broker). The messagee_id is used to identify the message(qos 1). 
Only works with QoS 0. The QoS 1 and 2 are not implemented yet. The retain and dup are not implemented yet.
*/
int serialize_publish(packet_type_code_t packet_type, char *buffer, size_t buffer_size, const char *topic, char *message, uint16_t message_id, uint8_t qos, uint8_t retain, uint8_t dup);

// Function to serialize publish packets(broker->client). Acknowledge the reception of a publish message. The message id is used to identify the message(publish message).
int serialize_puback(packet_type_code_t packet_type, char *buffer, size_t buffer_size, uint16_t message_id);

// Function to serialize pubrec packets(broker->client). Acknowledge the reception of a publish message with QoS 2. The message id is used to identify the message(publish message).
int serialize_pubrec(packet_type_code_t packet_type, char *buffer, size_t buffer_size, uint16_t message_id);

// Function to serialize pubrel packets(client->broker). Acknowledge the reception of a pubrec message. The message id is used to identify the message(pubrec message).
int serialize_pubrel(packet_type_code_t packet_type, char *buffer, size_t buffer_size, uint16_t message_id);

// Function to serialize pubcomp packets(broker->client). Acknowledge the reception of a pubrel message. The message id is used to identify the message(pubrel message).
int serialize_pubcomp(packet_type_code_t packet_type, char *buffer, size_t buffer_size, uint16_t message_id);

// Function to serialize subscribe packets(client->broker). The message_id is used to identify the message.
int serialize_subscribe(packet_type_code_t packet_type, char *buffer, size_t buffer_size, const char *topic, uint16_t message_id, uint8_t qos);


// Function to deserialize subscribe packets(broker->client). This function receive PUBLISH packets from broker and print the message in the terminal.
int handle_publish(int socket_fd) ;

// Function to serialize suback packets(broker->client). Acknowledge the reception of a subscribe message. The message id is used to identify the message(subscribe message).
int serialize_suback(packet_type_code_t packet_type, char *buffer, size_t buffer_size, uint16_t message_id, uint8_t qos);

// Function to serialize unsubscribe packets(client->broker). The message_id is used to identify the message.
int serialize_unsubscribe(packet_type_code_t packet_type, char *buffer, size_t buffer_size, const char *topic, uint16_t message_id);

// Function to serialize unsuback packets(broker->client). Acknowledge the reception of a unsubscribe message. The message id is used to identify the message(unsubscribe message).
int serialize_unsuback(packet_type_code_t packet_type, char *buffer, size_t buffer_size, uint16_t message_id);

// Function to serialize pingreq packets(client->broker)
int serialize_pingreq(packet_type_code_t packet_type, char *buffer, size_t buffer_size);

// Function to serialize pingresp packets(broker->client)
int serialize_pingresp(packet_type_code_t packet_type, char *buffer, size_t buffer_size);

// Function to serialize disconnect packets(client->broker)
int serialize_disconnect(packet_type_code_t packet_type, char *buffer, size_t buffer_size);

#endif