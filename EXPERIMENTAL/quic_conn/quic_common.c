#include "quic_common.h"


void generate_cid(ngtcp2_cid *cid, size_t len){
    if (len > NGTCP2_MAX_CIDLEN) {
        len = NGTCP2_MAX_CIDLEN; // Evita buffer overflow(limitando o tamanho dele)
    }
    cid->datalen = len;
    memset(cid->data, 0, sizeof(cid->data));
    for (size_t i = 0; i < len; i++) {
        cid->data[i] = rand() % 256;
    }
}

ngtcp2_path *init_ngtcp2_path(const char *local_ip, uint16_t local_port,const char *remote_ip, uint16_t remote_port){
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

void free_ngtcp2_path(ngtcp2_path *path){
    if (path) {
        free(path->local.addr);
        free(path->remote.addr);
        free(path);
    }
}

void custom_ngtcp2_rand(uint8_t *dest, size_t destlen, const ngtcp2_rand_ctx *rand_ctx) {
    (void) rand_ctx;
    //srand((unsigned int)time(NULL)); caso queira inicializar
    for (size_t i = 0; i < destlen; i++) {
        dest[i] = (uint8_t)rand();
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


void define_callbacks(ngtcp2_callbacks *callback, void *user_data){
    
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

ngtcp2_tstamp timestamp_now() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ngtcp2_tstamp)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

ngtcp2_conn *my_get_conn(ngtcp2_crypto_conn_ref *ref) {
    return (ngtcp2_conn *)ref->user_data; // Cast para garantir compatibilidade
}

void create_wssl_init_api_client(WOLFSSL_CTX* ctx,WOLFSSL_QUIC_METHOD quic_method){
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

void create_wssl_init_api_server(WOLFSSL_CTX* ctx,WOLFSSL_QUIC_METHOD quic_method){
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