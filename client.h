#ifndef CLIENT_H
#define CLIENT_H

/*

//configurações do cliente. Aqui contem as configurações pertinentes para a conexao a nivel de aplicação
struct client_config{
    id;
    tempo de conexao;
    keep_alive;
    ip_broker;
    port_broker;
    default_qos;
}

//estrutura do cliente. Aqui contem as configuracoes pertinentes para a conexao a nivel de transporte
struct client{
    socket;
    buffer;
    buffer_size;

    client_config config; // struct client_config
}

//estrutura da mensagem. Aqui contem as informacoes da mensagem a ser enviada
struct message{
    topic;
    payload;
    qos;
    retain;
}


// Funções básicas para uma comunicação 
int create_client(struct client *client, struct client_config *config);
int destroy_client(struct client *client); // Fecha o socket e libera a memória 
int connect_client(struct client *client); // Conecta o cliente ao broker
int disconnect_client(struct client *client); // Desconecta o cliente do broker
int send_message(struct client *client, struct message *msg); // Envia uma mensagem ao broker. O cliente deve estar conectado. Aqui seria feito a serialização da mensagem e enviada a nível de transporte 
int receive_message(struct client *client, struct message *msg); // Recebe uma mensagem do broker. O cliente deve estar conectado. Aqui seria feito a desserialização da mensagem recebida a nível de transporte(um subscribe por exemplo).
int subscribe_topic(struct client *client, char *topic); // Inscreve o cliente em um tópico
int unsubscribe_topic(struct client *client, char *topic); // Cancela a inscrição do cliente em um tópico
int keep_alive(struct client *client); // Envia um PINGREQ ao broker. Seria pra indicar que o cliente ainda está ativo. O broker deve responder com PINGRESP

//getters e setters

int set_qos(struct client *client, int qos); // Define o QoS padrão do cliente
int get_qos(struct client *client, int *qos); // Obtém o QoS padrão do cliente

int set_keep_alive(struct client *client, int keep_alive); // Define o tempo de keep alive do cliente
int set_broker(struct client *client, char *ip, int port); // Define o endereço do broker

int set_socket(struct client *client, int socket); // Define o socket do cliente
int get_socket(struct client *client, int *socket); // Obtém o socket do cliente

int set_id(struct client *client, char *id); // Define o ID do cliente
int get_id(struct client *client, char *id); // Obtém o ID do cliente

int set_buffer_size(struct client *client, size_t buffer_size); // Define o tamanho do buffer
int get_buffer_size(struct client *client, size_t *buffer_size); // Obtém o tamanho do buffer

int set_buffer(struct client *client, void *buffer); // Define o buffer
int get_buffer(struct client *client, void *buffer); // Obtém o buffer

int set_config(struct client *client, struct client_config *config); // Define a configuração do cliente
int get_config(struct client *client, struct client_config *config); // Obtém a configuração do cliente

int set_buffer(struct client *client, void *buffer); // Define o buffer
int get_buffer(struct client *client, void *buffer); // Obtém o buffer

int get_socket(struct client *client, int *socket); // Obtém o socket do cliente
int get_config(struct client *client, struct client_config *config); // Obtém a configuração do cliente





*/

#endif // CLIENT_H