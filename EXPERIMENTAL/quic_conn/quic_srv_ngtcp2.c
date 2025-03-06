#define WOLFSSL_QUIC
#include <ngtcp2/ngtcp2_crypto.h>
#include <signal.h>
#include <ngtcp2/ngtcp2_crypto_wolfssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ngtcp2/ngtcp2.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wolfssl/options.h>
#include <wolfssl/ssl.h> 
#include <wolfssl/quic.h>


#define DCID_LEN 18 //LENGTH OF DCID(test)

//Embaixo temos a funcao que vai tratar o sinal de interrupcao
volatile sig_atomic_t stop = 0;

void signal_handler(int signo) {
    if (signo == SIGINT) {
        printf("\nCtrl+C detectado. Encerrando o programa...\n");
        stop = 1;
    }
}

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
    if(ngtcp2_crypto_wolfssl_configure_server_context(ctx)!=0){
        fprintf(stderr, "Erro ao configurar o contexto do cliente\n");
        return;
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

//Callback to configure the server context(initially made to creation of TLS context)
static ngtcp2_conn *get_conn(ngtcp2_crypto_conn_ref *conn_ref) {
    return (ngtcp2_conn *)conn_ref->user_data;
}

void generate_cid(ngtcp2_cid *cid, size_t len) {
    if (len > NGTCP2_MAX_CIDLEN) {
        len = NGTCP2_MAX_CIDLEN; // Evita buffer overflow(limitando o tamanho dele)
    }
    cid->datalen = len;
    for (size_t i = 0; i < len; i++) {
        cid->data[i] = rand() % 256;
    }
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



int get_new_cid(ngtcp2_conn *conn, ngtcp2_cid *cid, uint8_t *token, size_t cidlen, void *user_data){
    (void)conn;//To avoid warning
    (void)user_data;//To avoid warning
    for (size_t i = 0; i < cidlen; ++i) {
        cid->data[i] = rand() % 256;  
    }
    cid->datalen = cidlen;
    
    for (size_t i = 0; i < NGTCP2_STATELESS_RESET_TOKENLEN; ++i) {
        token[i] = rand() % 256;  
    }

    return 0;  
}


void define_callbacks(ngtcp2_callbacks *callback, void *user_data)
{

    callback->recv_client_initial = ngtcp2_crypto_recv_client_initial_cb;
    callback->recv_crypto_data = ngtcp2_crypto_recv_crypto_data_cb;
    callback->encrypt = ngtcp2_crypto_encrypt_cb;
    callback->decrypt = ngtcp2_crypto_decrypt_cb;
    callback->hp_mask = ngtcp2_crypto_hp_mask_cb;
    callback->rand = custom_ngtcp2_rand;
    callback->get_new_connection_id = get_new_cid;
    callback->update_key = ngtcp2_crypto_update_key_cb;
    callback->delete_crypto_aead_ctx = ngtcp2_crypto_delete_crypto_aead_ctx_cb;
    callback->delete_crypto_cipher_ctx = ngtcp2_crypto_delete_crypto_cipher_ctx_cb;
    callback->get_path_challenge_data = ngtcp2_crypto_get_path_challenge_data_cb;
    callback->version_negotiation = ngtcp2_crypto_version_negotiation_cb;
}

int main(){
    srand((unsigned int)time(NULL));
    ngtcp2_conn *conn;

    ngtcp2_settings settings;

    ngtcp2_settings_default(&settings);

    ngtcp2_transport_params params;

    uint32_t client_chosen_version;

    ngtcp2_callbacks callbacks;
    define_callbacks(&callbacks,NULL);

    client_chosen_version = NGTCP2_PROTO_VER_V1;
    ngtcp2_transport_params_default(&params);

    int srv;
    ngtcp2_cid dcid, scid;
    ngtcp2_path *path;
    path = init_ngtcp2_path("127.0.0.1", 12345, "127.0.0.1", 12346);

    generate_cid(&dcid, DCID_LEN);
    generate_cid(&scid, DCID_LEN);


    params.original_dcid = dcid;
    //If is a server, this param(original_dcid) is True
    params.original_dcid_present = 1;

    srv = ngtcp2_conn_server_new(&conn,&dcid,&scid,path,client_chosen_version,&callbacks,&settings,&params,NULL,NULL);

    if (srv != 0) {
        fprintf(stderr, "Erro ao criar conexão: %d\n", srv);
        return EXIT_FAILURE;
    }
    printf("Server QUIC criada com sucesso!\n");

    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Erro ao configurar o sinal SIGINT\n");
        return EXIT_FAILURE;
    }

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
    
    struct sockaddr_in *remote_addr = (struct sockaddr_in *)path->remote.addr;
    //==================================================================================================
    printf("Endereço remoto: %s:%d\n", inet_ntoa(remote_addr->sin_addr), ntohs(remote_addr->sin_port));


    printf("\nVersão escolhida do srv: 0x%08x\n", client_chosen_version);
    printf("Resultado da criação da conexão: %d\n", srv);

    char *algorithm;
    if(settings.cc_algo == 1){
        algorithm = "CUBIC";
    }
    printf("Algoritmo de congestionamento(valor): %u\n", settings.cc_algo); //Se printar 1 significa que eh CUBIC
    printf("Algoritmo de congestionamento: %s\n", algorithm);
    

    ngtcp2_crypto_conn_ref *server_conn_ref = malloc(sizeof(ngtcp2_crypto_conn_ref));
    if (!server_conn_ref) {
        fprintf(stderr, "Falha ao alocar memória para server_conn_ref\n");
        free(server_conn_ref);
        return EXIT_FAILURE;
    }
    server_conn_ref->get_conn = my_get_conn;//Alocando a conexao do srv para o cb

    server_conn_ref->user_data = conn;

    ngtcp2_conn *retrieved_conn = server_conn_ref->get_conn(server_conn_ref);
    printf("Conexão recuperada(endereco da memoria): %p\n", (void *)retrieved_conn);

    WOLFSSL_CTX* ctx;
    const WOLFSSL_QUIC_METHOD quic_method = {
        .set_encryption_secrets = my_set_encryption_secrets,
        .add_handshake_data = my_add_handshake_data,
        .flush_flight = my_flush_flight,
        .send_alert = my_send_alert
    };
    //alocando esse ctx
    create_wssl_init_api(ctx,quic_method);



    free_ngtcp2_path(path);
    ngtcp2_conn_del(conn);
   

    return EXIT_SUCCESS;
}

