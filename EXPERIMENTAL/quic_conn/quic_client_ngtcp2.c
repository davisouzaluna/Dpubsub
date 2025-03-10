#define WOLFSSL_QUIC
#define _POSIX_C_SOURCE 200809L
#include <ngtcp2/ngtcp2_crypto.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ngtcp2/ngtcp2.h>
#include <ngtcp2/ngtcp2_crypto_wolfssl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <string.h>

#include <wolfssl/options.h>
//os headers abaixo ja estao importados em:<ngtcp2/ngtcp2_crypto_wolfssl.h> mas eu estou importando de novo
#include <wolfssl/ssl.h> 
#include <wolfssl/quic.h>

#define MAX_SECRET_LEN 64



#define DCID_LEN 18 //LENGTH OF DCID(test)

//cria e aloca com o ngtcp2(api) o contexto ssl do cliente
void create_wssl_init_api(WOLFSSL_CTX* ctx,WOLFSSL_QUIC_METHOD quic_method){
    wolfSSL_Init();
    if((ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method())) == NULL){
        fprintf(stderr, "Erro ao criar o contexto do cliente\n");
        return;
    }
    if(wolfSSL_CTX_set_quic_method(ctx, &quic_method)!=WOLFSSL_SUCCESS){
        fprintf(stderr, "Erro ao configurar o metodo QUIC\n");
        return;
    }
    else{
        printf("Metodo QUIC configurado com sucesso!\n");
    }
    //ngtcp2 API
    if(ngtcp2_crypto_wolfssl_configure_client_context(ctx)!=0){
        fprintf(stderr, "Erro ao configurar o contexto do cliente\n");
        return;
    }else{
        printf("Contexto do cliente configurado com sucesso!\n");
    }
    //CA
    if (wolfSSL_CTX_load_verify_locations(ctx,"../cert/server.crt",0) !=
    SSL_SUCCESS) {
    fprintf(stderr, "Error loading cert/server.crt, please check"
    "the file. :/\n");
    exit(EXIT_FAILURE);
    }
    else{
        printf("Certificado carregado com sucesso!\n");
    }

    /*
    TODO: criar funcao para desalocar esses recursos, ou chamar no final da main
    */
}
void custom_ngtcp2_rand(uint8_t *dest, size_t destlen, const ngtcp2_rand_ctx *rand_ctx) {
    (void) rand_ctx;
    //srand((unsigned int)time(NULL)); caso queira inicializar
    for (size_t i = 0; i < destlen; i++) {
        dest[i] = (uint8_t)rand();
    }
}

void generate_cid(ngtcp2_cid *cid, size_t len) {
    if (len > NGTCP2_MAX_CIDLEN) {
        len = NGTCP2_MAX_CIDLEN; // Evita buffer overflow(limitando o tamanho dele)
    }
    cid->datalen = len;
    memset(cid->data, 0, sizeof(cid->data));
    for (size_t i = 0; i < len; i++) {
        cid->data[i] = rand() % 256;
    }
}

int get_new_cid(ngtcp2_conn *conn, ngtcp2_cid *cid, uint8_t *token, size_t cidlen, void *user_data) {
    if (cidlen > NGTCP2_MAX_CIDLEN) {
        return -1; // Evita buffer overflow
    }
    cid->datalen = cidlen;
    memset(cid->data, 0, sizeof(cid->data));
    for (size_t i = 0; i < cidlen; ++i) {
        cid->data[i] = rand() % 256;
    }
    memset(token, 0, NGTCP2_STATELESS_RESET_TOKENLEN);
    for (size_t i = 0; i < NGTCP2_STATELESS_RESET_TOKENLEN; ++i) {
        token[i] = rand() % 256;
    }
    return 0;
}

ngtcp2_conn *my_get_conn(ngtcp2_crypto_conn_ref *ref) {
    return (ngtcp2_conn *)ref->user_data; // Cast para garantir compatibilidade
}


ngtcp2_path *init_ngtcp2_path(const char *local_ip, uint16_t local_port,
                              const char *remote_ip, uint16_t remote_port) {
    ngtcp2_path *path = malloc(sizeof(ngtcp2_path));
    if (!path) {
        return NULL; 
    }

    memset(path, 0, sizeof(ngtcp2_path));

    
    struct sockaddr_in *local_addr = malloc(sizeof(struct sockaddr_in));
    if (!local_addr) {
        free(path);
        return NULL;
    }
    memset(local_addr, 0, sizeof(struct sockaddr_in));
    local_addr->sin_family = AF_INET;
    local_addr->sin_port = htons(local_port);
    local_addr->sin_addr.s_addr = inet_addr(local_ip);

    struct sockaddr_in *remote_addr = malloc(sizeof(struct sockaddr_in));
    if (!remote_addr) {
        free(local_addr);
        free(path);
        return NULL;
    }
    memset(remote_addr, 0, sizeof(struct sockaddr_in));
    remote_addr->sin_family = AF_INET;
    remote_addr->sin_port = htons(remote_port);
    remote_addr->sin_addr.s_addr = inet_addr(remote_ip);

    path->local.addr = (struct sockaddr *)local_addr;
    path->local.addrlen = sizeof(struct sockaddr_in);
    path->remote.addr = (struct sockaddr *)remote_addr;
    path->remote.addrlen = sizeof(struct sockaddr_in);

    return path;
}


void free_ngtcp2_path(ngtcp2_path *path) {
    if (path) {
        free(path->local.addr);
        free(path->remote.addr);
        free(path);
    }
}

//==================================TLS stack(nenhum ta funcionando, so poc)

int my_set_encryption_secrets(WOLFSSL* ssl, WOLFSSL_ENCRYPTION_LEVEL level,
    const uint8_t* read_secret,
    const uint8_t* write_secret,
    size_t secret_len) {
printf("Setting encryption secrets for level %d\n", level);

if (read_secret) {
printf("Read secret set.\n");
}
if (write_secret) {
printf("Write secret set.\n");
}

return WOLFSSL_SUCCESS;
}

/* Callback para adicionar dados de handshake ao QUIC */
int my_add_handshake_data(WOLFSSL* ssl, WOLFSSL_ENCRYPTION_LEVEL level,
const uint8_t* data, size_t len) {
printf("Adding handshake data for level %d, length: %zu\n", level, len);
return WOLFSSL_SUCCESS;
}

/* Callback para limpar pacotes armazenados em buffer */
int my_flush_flight(WOLFSSL* ssl) {
printf("Flushing flight (sending buffered handshake packets)\n");
return WOLFSSL_SUCCESS;
}

/* Callback para enviar um alerta TLS no QUIC */
int my_send_alert(WOLFSSL* ssl, WOLFSSL_ENCRYPTION_LEVEL level,
uint8_t alert) {
printf("Sending QUIC alert (level %d): %d\n", level, alert);
return WOLFSSL_SUCCESS;
}
//==================================


void define_callbacks(ngtcp2_callbacks *callback, void *user_data)
{

    callback->client_initial = ngtcp2_crypto_client_initial_cb;
    callback->recv_crypto_data = ngtcp2_crypto_recv_crypto_data_cb;
    callback->encrypt = ngtcp2_crypto_encrypt_cb;
    callback->decrypt = ngtcp2_crypto_decrypt_cb;
    callback->hp_mask = ngtcp2_crypto_hp_mask_cb;
    callback->recv_retry = ngtcp2_crypto_recv_retry_cb;
    callback->rand = custom_ngtcp2_rand;
    callback->get_new_connection_id = get_new_cid;
    callback->delete_crypto_aead_ctx = ngtcp2_crypto_delete_crypto_aead_ctx_cb;
    callback->delete_crypto_cipher_ctx = ngtcp2_crypto_delete_crypto_cipher_ctx_cb;
    callback->get_path_challenge_data = ngtcp2_crypto_get_path_challenge_data_cb;
    callback->version_negotiation = ngtcp2_crypto_version_negotiation_cb;
    callback->update_key = ngtcp2_crypto_update_key_cb;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define TIMEOUT_SEC 2

int establish_udp_connection(const char *server_ip, int server_port) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);
    
    // Criar socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar socket UDP");
        return -1;
    }

    // Configurar endereço do servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    // Mensagem de teste antes da conexão QUIC
    const char *msg = "Hello UDP";
    if (sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao enviar mensagem UDP");
        close(sockfd);
        return -1;
    }

    printf("Mensagem UDP enviada para %s:%d\n", server_ip, server_port);

    // Configurar timeout para receber resposta
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // Aguardar resposta do servidor
    int recv_len = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (recv_len < 0) {
        perror("Nenhuma resposta do servidor UDP");
        close(sockfd);
        return -1;
    }

    buffer[recv_len] = '\0'; // Terminar a string recebida
    printf("Resposta do servidor UDP: %s\n", buffer);

    close(sockfd);
    return 0;
}


//hora atual, pra alocar pro tipo ngtcp2_tstamp, que eh um uint64_t
ngtcp2_tstamp timestamp_now() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ngtcp2_tstamp)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

int main(){
    ngtcp2_conn *conn;
    srand((unsigned int)time(NULL));
    ngtcp2_settings settings;

    ngtcp2_settings_default(&settings);

    ngtcp2_transport_params params;

    uint32_t client_chosen_version;

    ngtcp2_callbacks callbacks;
    define_callbacks(&callbacks,NULL);

    client_chosen_version = NGTCP2_PROTO_VER_V1;
    ngtcp2_transport_params_default(&params);

    //=======================Alocando outros parametros pra conexao
    params.initial_max_stream_data_bidi_local = 65536;
    params.initial_max_stream_data_bidi_remote = 65536;
    params.initial_max_stream_data_uni = 65536;
    params.initial_max_data = 1048576;
    params.initial_max_streams_bidi = 200;
    params.initial_max_streams_uni = 100;
    params.max_idle_timeout = 30000;
    params.max_udp_payload_size = 1350;
    params.active_connection_id_limit = 4;
    params.ack_delay_exponent = 3;
    params.max_ack_delay = 25;
    params.max_datagram_frame_size = 1350;
    params.disable_active_migration = 1;
    params.grease_quic_bit = 1;

    params.original_dcid_present = 0;
    params.initial_scid_present = 0;
    params.retry_scid_present = 0;
    params.preferred_addr_present = 0;
    params.stateless_reset_token_present = 0;
//=================================================================================
    int cliente;
    ngtcp2_cid dcid, scid;
    ngtcp2_path *path = init_ngtcp2_path("127.0.0.1", 12345, "127.0.0.1", 12346);

    if (!path) {
        printf("Erro ao inicializar ngtcp2_path\n");
        return -1;
    }

    generate_cid(&dcid, DCID_LEN);
    generate_cid(&scid, DCID_LEN);

    cliente = ngtcp2_conn_client_new(&conn,&dcid,&scid,path,client_chosen_version,&callbacks,&settings,&params,NULL,NULL);

    if (cliente != 0) {
        fprintf(stderr, "Erro ao criar conexão: %d\n", cliente);
        return EXIT_FAILURE;
    }

    printf("Client QUIC criada com sucesso!\n");
    printf("Endereco de memoria da conexao QUIC: %p\n", (void *)conn);
    /*
    ======================================================
    
    Here I will print the parameters for correction
    */
    printf("DCID: ");
    for (size_t i = 0; i < dcid.datalen; i++) {
    printf("%02x", dcid.data[i]);
    }
    printf("\n");

    printf("SCID: ");
    for (size_t i = 0; i < scid.datalen; i++) {
    printf("%02x", scid.data[i]);
    }
    printf("\n");

    //=====================================================debug dos enderecos
    struct sockaddr_in *local_addr = (struct sockaddr_in *)path->local.addr;
    
    printf("Endereço local: %s:%d\n", inet_ntoa(local_addr->sin_addr), ntohs(local_addr->sin_port));

    printf("Versão escolhida do cliente: 0x%08x\n", client_chosen_version);
    printf("Resultado da criação da conexão: %d\n", cliente);

    printf("settings.cc/ algoritmo: %u\n", settings.cc_algo); //Se printar 1 significa que eh CUBIC
    

    ngtcp2_crypto_conn_ref *client_conn_ref = malloc(sizeof(ngtcp2_crypto_conn_ref));
    if (!client_conn_ref) {
        fprintf(stderr, "Falha ao alocar memória para client_conn_ref\n");
        free(client_conn_ref);
        return EXIT_FAILURE;
    }
    client_conn_ref->get_conn = my_get_conn;//Alocando a conexao do cliente para o cb


    client_conn_ref->user_data = conn; 
    //=====================================================debug das conexoes(referencia delas)
    ngtcp2_conn *retrieved_conn = client_conn_ref->get_conn(client_conn_ref);
    printf("Conexão recuperada(endereco da memoria): %p\n", (void *)retrieved_conn);

    //=====================================================criacao de um ctx pra conexao TLS
    WOLFSSL_CTX* ctx;
    const WOLFSSL_QUIC_METHOD quic_method = {
        .set_encryption_secrets = my_set_encryption_secrets,
        .add_handshake_data = my_add_handshake_data,
        .flush_flight = my_flush_flight,
        .send_alert = my_send_alert
    };
    //alocando esse ctx
    create_wssl_init_api(ctx,quic_method);

    /*
    TODO: function wolfSSL_CTX_set_quic_method 
    https://www.wolfssl.com/documentation/manuals/wolfssl/wolfSSL-Manual.pdf
    page 1216
    */
   
    
    //criacao de uma stream
    //buffer pro pkt
    printf("Tamanho maximo do payload: %zu\n", settings.max_tx_udp_payload_size);
    size_t buffer_len = settings.max_tx_udp_payload_size;
    printf("Tamanho do buffer: %zu\n", buffer_len);
    uint8_t buffer[buffer_len];
    printf("Tamanho do buffer: %zu\n", sizeof(buffer));
    ngtcp2_vec data_vec;
    printf("Tamanho do buffer da estrutura do tipo ngtcp2_vec: %zu\n", sizeof(data_vec));
    uint32_t flags = 0;
    //teste das funcoes
    ngtcp2_ssize write_pkt;
    
    ngtcp2_ssize ngtcp2_conn_writev_stream_fn;
    //=====================================================
    ngtcp2_tstamp ts = timestamp_now();
   
    ngtcp2_pkt_info *pkt_info = malloc(sizeof(ngtcp2_pkt_info));
    memset(pkt_info, 0, sizeof(pkt_info));
    pkt_info->ecn = NGTCP2_ECN_NOT_ECT;


    printf("criando stream...\n");
    
    int64_t pstream_id;
    
    typedef struct {
        int id;
        char buffer[1024];
    } stream_context;
    
    stream_context ctx_teste;
    ctx_teste.id = 123;
    strcpy(ctx_teste.buffer, "Mensagem de teste");
    
    struct sockaddr_in *remote_addr = (struct sockaddr_in *)path->remote.addr;
    char server_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(remote_addr->sin_addr), server_ip, INET_ADDRSTRLEN);
    int server_port = ntohs(remote_addr->sin_port);

    printf("Endereço remoto: %s:%d\n", server_ip, server_port);
    /*
    int stream_id = ngtcp2_conn_open_bidi_stream(conn,&pstream_id,&ctx_teste);
    
    if (stream_id<0) {
        printf("stream_id: %d\n", stream_id);
        fprintf(stderr, "Erro ao abrir stream: %d,(%s)\n", stream_id, ngtcp2_strerror((int)stream_id));
        return EXIT_FAILURE;
    }
    //=====================================================
    printf("Enviando dados...\n");
    if (establish_udp_connection(server_ip, server_port) < 0) {
        fprintf(stderr, "Falha ao estabelecer conexão UDP\n");
        return EXIT_FAILURE;
    }

    */
    ngtcp2_ssize result = ngtcp2_conn_writev_stream(conn,path,pkt_info,buffer,sizeof(buffer),NULL,flags,0,&data_vec,1,ts);
    if (result < 0) {
        fprintf(stderr, "Erro ao escrever stream: %zd (%s)\n", result, ngtcp2_strerror((int)result));
        free(path);
        return EXIT_FAILURE;
    }
    printf("Dados enviados: %zd bytes\n", ngtcp2_conn_writev_stream_fn);
    
    //=====================================================
    free_ngtcp2_path(path);
    free(client_conn_ref);
    ngtcp2_conn_del(conn);
    return EXIT_SUCCESS;
}

