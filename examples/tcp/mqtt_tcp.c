#include "../../lib/src/include/dpubsub.h"
#include <stdio.h>
#include <stdint.h>

int main() {
    client_t client;
    client_config_t config;
    size_t buffer_size = 0; // Tamanho do buffer será definido pelo padrão se for 0

    
    char *client_id = "mqtt_TCP_client";
    uint16_t keep_alive_interval = 60; 
    char *ip_broker = "35.172.255.228";
    uint16_t port_broker = 1883;
    uint8_t default_qos = 0;
    char *topic = "dpubsub";
    uint16_t message_id = 1;

    
    if (dpubsub_define_config_struct(&config, client_id, keep_alive_interval, ip_broker, port_broker, default_qos) != 0) {
        fprintf(stderr, "Erro ao definir a configuração do cliente.\n");
        return 1;
    }

    
    if (dpubsub_config(&client, &config, buffer_size, DPUBSUB_TCP) != 0) {
        fprintf(stderr, "Erro ao configurar o cliente.\n");
        return 1;
    }

 
    if (dpubsub_connect(&client, DPUBSUB_TCP) != 0) {
        fprintf(stderr, "Erro ao conectar ao broker.\n");
        return 1;
    }

    
    if (dpubsub_subscribe(&client, topic, message_id, DPUBSUB_TCP) != 0) {
        fprintf(stderr, "Erro ao se inscrever no tópico.\n");
        return 1;
    }

    printf("Cliente conectado e inscrito com sucesso no tópico '%s'.\n", topic);
    return 0;
}
