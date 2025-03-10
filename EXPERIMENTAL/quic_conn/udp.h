#ifndef UDP_H
#define UDP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para criar o socket UDP. Retorna 0 em caso de sucesso, -1 em caso de erro
int create_udp_socket();

// Função para vincular o socket UDP a um endereço e porta. Retorna 0 em caso de sucesso, -1 em caso de erro
int bind_udp_socket(int sockfd, const char *ip, int port);

// Função para enviar dados via UDP. Retorna o número de bytes enviados em caso de sucesso, -1 em caso de erro
ssize_t send_udp_packet(int sockfd, const void *data, size_t data_len, const char *dest_ip, int dest_port);

// Função para receber dados via UDP. Retorna o número de bytes recebidos em caso de sucesso, -1 em caso de erro
ssize_t receive_udp_packet(int sockfd, void *buffer, size_t buffer_len, struct sockaddr_in *src_addr);

// Função para fechar o socket UDP.
void close_udp_socket(int sockfd);

#endif