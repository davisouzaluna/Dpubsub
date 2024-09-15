// tcp_server.h
#include <stddef.h>
#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#define INITIAL_BUFFER_SIZE 1024 // Tamanho inicial do buffer de recebimento

// Cria e configura o socket de escuta. A funcao retorna o descritor do socket de escuta
int create_and_listen_server(int port);

// Aceita uma conexão do cliente. A funcao retorna o descritor do socket da conexao
int accept_client(int sockfd);

// Fecha a conexão com o cliente
int close_connection_server(int connfd);

// Fecha o socket de escuta
int close_socket_server(int sockfd);

// Recebe bytes do cliente. A função aloca um buffer dinâmico para armazenar os bytes recebidos
void receive_bytes_from_client(int connfd, void *buffer, size_t buffer_size);
//void *manage_buffer(void *buffer, size_t *buffer_size, size_t required_size); //essa função e acessada pelo receive_bytes_with_dynamic_buffer. Por enquanto nao deve ficar disponivel para toda a logica de negocio

// Função que recebe os dados e ajusta o buffer dinamicamente. Não é necessário fornecer um buffer.
void receive_bytes_with_dynamic_buffer(int connfd);

// Função que recebe os dados e armazena em um buffer estático. O buffer deve ser fornecido pelo usuário.
int receive_bytes_from_client_static_buff(int connfd, void *buffer, size_t buffer_size) ;

/* Função para enviar bytes ao cliente. Ela e semelhante a funcao de envio de tcp_client. Todas elas enviam pelo mesmo metodo de envio(funcao send do tcp)
 **AINDA NAO TESTADA**  */
void send_bytes_to_client(int connfd, const void *buffer, size_t buffer_size);//para uma possivel resposta ao cliente

// Fecha um recurso/Debug
int close_resource(int fd);

#endif // TCP_SERVER_H
