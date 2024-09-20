#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "client.h"
#include "packets.h"

int main() {
    client_t client;
    client_config_t config;
    size_t buffer_size = 0; // Tamanho do buffer será definido pelo padrão se for 0

    // Configuração do cliente
    config.client_id = "davifurao";
    config.keep_alive = 60;
    config.ip_broker = "91.121.93.94"; // IP de teste
    config.port_broker = 1883; // Porta do servidor de teste: ../transport/tcp/tests/server.c
    config.default_qos = 0;



    // Testar a criação do cliente
    if (create_client(&client, &config, buffer_size) != 0) {
        fprintf(stderr, "Failed to create client\n");
        return EXIT_FAILURE;
    }
    printf("Client created successfully\n");

    // Testar a conexão com o broker usando TCP
    if (connect_client(&client, PROTOCOL_TCP) != 0) {
        fprintf(stderr, "Failed to connect client\n");
        destroy_client(&client); // Certifique-se de liberar os recursos em caso de falha na conexão
        return EXIT_FAILURE;
    }
    printf("Client connected successfully\n");
    /*
    // Testar o envio de dados para o broker
    char *msg = "Hello, server!";
    message_t message;
    message.topic = "test_topic";
    message.payload = msg;
    message.qos = 1;
    message.retain = 0;
    if (send_message(&client, &message, PROTOCOL_TCP) != 0) {
        fprintf(stderr, "Failed to send message\n");
        disconnect_client(&client, PROTOCOL_TCP); // Certifique-se de desconectar o cliente em caso de falha no envio da mensagem
        destroy_client(&client); // Certifique-se de liberar os recursos em caso de falha no envio da mensagem
        return EXIT_FAILURE;
    }
    
    int received_message = receive_message(&client, PROTOCOL_TCP);
    //testar o recebimento de dados do broker
    if (received_message<0) {
        fprintf(stderr, "Failed to receive message\n");
        disconnect_client(&client, PROTOCOL_TCP); // Certifique-se de desconectar o cliente em caso de falha no recebimento da mensagem
        destroy_client(&client); // Certifique-se de liberar os recursos em caso de falha no recebimento da mensagem
        return EXIT_FAILURE;
    }
    message_t received;
    */
    // testar a serializacao e envio da mensagem(observar pelo Wireshark):
    //redimensionar o buffer para o tamanho do pacote
    char buffer_2[128];
    

    
    // Serializar a mensagem(testando alguns getterrs)
    int serialize_CONNECT = serialize_connect(CONNECT, buffer_2, sizeof(buffer_2), get_client_id(&client), get_keep_alive(&client), 0x04);

    if (serialize_CONNECT > 0) {
        printf("Pacote CONNECT serializado com sucesso! Tamanho: %d bytes\n", serialize_CONNECT);
        // Exibir o buffer em formato hexadecimal para verificação
        for (int i = 0; i < serialize_CONNECT; i++) {
            printf("%02X ", (unsigned char)buffer_2[i]);
        }
        printf("\n");
    } else {
        printf("Erro ao serializar o pacote CONNECT!\n");
    }

    // Enviar a mensagem
    if (send_bytes_to_server(get_socket_fd(&client), buffer_2, serialize_CONNECT) != 0) {
        fprintf(stderr, "Failed to send message\n");
        return EXIT_FAILURE;
    }

    printf("Socket FD: %d\n", get_socket_fd(&client));
    printf("Buffer: %p\n", buffer_2);
    printf("Bytes to send: %d\n", serialize_CONNECT);

    char buffer3[256];

    // testar a publicacao de dados para o broker
    const char *topic = "dpubsub";
    const char *msg = "opa1";
    uint16_t message_id = 1; // Usado apenas se QoS > 0
    uint8_t qos = get_qos(&client); // Nível de QoS(testando o getter)
    uint8_t retain = 0; // Não reter a mensagem
    uint8_t dup = 0; // Não duplicar a mensagem


    

    int packet_length = serialize_publish(PUBLISH, buffer3, sizeof(buffer3), topic, msg, message_id, qos, retain, dup);

    if (packet_length > 0) {
        printf("Pacote PUBLISH serializado com sucesso! Tamanho: %d bytes\n", packet_length);
        
        // Exibir o buffer em formato hexadecimal para verificação
        printf("Buffer: ");
        for (int i = 0; i < packet_length; i++) {
            printf("%02X ", (unsigned char)buffer3[i]);
        }
        printf("\n");
    } else {
        fprintf(stderr, "Erro ao serializar o pacote PUBLISH: %d\n", packet_length);
    }
    
    // Enviar a mensagem
    if (send_bytes_to_server(get_socket_fd(&client), buffer3, packet_length) != 0) {
        fprintf(stderr, "Failed to send message\n");
        return EXIT_FAILURE;
    }

    // Testar o envio do pacote disconnect
    char buffer4[2];
    int packet_length2 = serialize_disconnect(DISCONNECT, buffer4, sizeof(buffer4));

    if (packet_length2 > 0) {
        printf("Pacote DISCONNECT serializado com sucesso! Tamanho: %d bytes\n", packet_length2);
        
        // Exibir o buffer em formato hexadecimal para verificação
        printf("Buffer: ");
        for (int i = 0; i < packet_length2; i++) {
            printf("%02X ", (unsigned char)buffer4[i]);
        }
        printf("\n");
    } else {
        fprintf(stderr, "Erro ao serializar o pacote DISCONNECT: %d\n", packet_length2);
    }

    printf("sending a PINGREQ packet\n");

    char buffer5[2];
    int packet_length3 = serialize_pingreq(PINGREQ, buffer5, sizeof(buffer5));

    if (packet_length3 > 0) {
        printf("Pacote PINGREQ serializado com sucesso! Tamanho: %d bytes\n", packet_length3);
        
        // Exibir o buffer em formato hexadecimal para verificação
        printf("Buffer: ");
        for (int i = 0; i < packet_length3; i++) {
            printf("%02X ", (unsigned char)buffer5[i]);
        }
        printf("\n");
    } else {
        fprintf(stderr, "Erro ao serializar o pacote PINGREQ: %d\n", packet_length3);
    }

    // Enviar a mensagem
    if (send_bytes_to_server(get_socket_fd(&client), buffer5, packet_length3) != 0) {
        fprintf(stderr, "Failed to send message\n");
        return EXIT_FAILURE;
    }

    char buffer6[256];
    int packet_length4 = serialize_subscribe(SUBSCRIBE, buffer6, sizeof(buffer6), topic, message_id, qos);

    if (packet_length4 > 0) {
        printf("Pacote SUBSCRIBE serializado com sucesso! Tamanho: %d bytes\n", packet_length4);
        
        // Exibir o buffer em formato hexadecimal para verificação
        printf("Buffer: ");
        for (int i = 0; i < packet_length4; i++) {
            printf("%02X ", (unsigned char)buffer6[i]);
        }
        printf("\n");
    } else {
        fprintf(stderr, "Erro ao serializar o pacote SUBSCRIBE: %d\n", packet_length4);
    }

    // Enviar a mensagem
    if (send_bytes_to_server(get_socket_fd(&client), buffer6, packet_length4) != 0) {
        fprintf(stderr, "Failed to send message\n");
        return EXIT_FAILURE;
    } 
    printf("Sleeping for 2 seconds\n");
    sleep(2);
    printf("sending DISCONNECT packet\n");
    // Enviar a mensagem
    if (send_bytes_to_server(get_socket_fd(&client), buffer4, packet_length2) != 0) {
        fprintf(stderr, "Failed to send message\n");
        return EXIT_FAILURE;
    }


    // Testar a desconexão do broker usando TCP
    if (disconnect_client(&client, PROTOCOL_TCP) != 0) {
        fprintf(stderr, "Failed to disconnect client\n");
        destroy_client(&client); // Certifique-se de liberar os recursos em caso de falha na desconexão
        return EXIT_FAILURE;
    }

/*
    if(free_message(&received)!=0){
        fprintf(stderr, "Failed to free message\n");
        return EXIT_FAILURE;
    }else{
        printf("Message freed successfully\n");
    }
    */
    printf("Client disconnected successfully\n");
    // Testar a destruição do cliente
    if (destroy_client(&client) != 0) {
        fprintf(stderr, "Failed to destroy client\n");
        return EXIT_FAILURE;
    }
    printf("Client destroyed successfully\n");

    return EXIT_SUCCESS;
}
