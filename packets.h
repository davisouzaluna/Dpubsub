#ifndef PACKETS_H
#define PACKETS_H

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

#endif