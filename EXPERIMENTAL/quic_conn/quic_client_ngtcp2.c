#include <ngtcp2/ngtcp2_crypto.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ngtcp2/ngtcp2.h>
#include <ngtcp2/ngtcp2_crypto_wolfssl.h>

#define DCID_LEN 18 //LENGTH OF DCID(test)


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
    for (size_t i = 0; i < len; i++) {
        cid->data[i] = rand() % 256;
    }
}

int get_new_cid(ngtcp2_conn *conn, ngtcp2_cid *cid, uint8_t *token, size_t cidlen, void *user_data){
    for (size_t i = 0; i < cidlen; ++i) {
        cid->data[i] = rand() % 256;  
    }
    cid->datalen = cidlen;
    
    for (size_t i = 0; i < NGTCP2_STATELESS_RESET_TOKENLEN; ++i) {
        token[i] = rand() % 256;  
    }

    return 0;  
}
ngtcp2_conn *my_get_conn(ngtcp2_crypto_conn_ref *ref) {
    return (ngtcp2_conn *)ref; // Cast para garantir compatibilidade
}


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

    int cliente;
    ngtcp2_cid dcid, scid;
    ngtcp2_path path;
    path.local.addr = NULL;  
    path.local.addrlen = 0;
    path.remote.addr = NULL;
    path.remote.addrlen = 0;

    generate_cid(&dcid, DCID_LEN);
    generate_cid(&scid, DCID_LEN);

    cliente = ngtcp2_conn_client_new(&conn,&dcid,&scid,&path,client_chosen_version,&callbacks,&settings,&params,NULL,NULL);

    if (cliente != 0) {
        fprintf(stderr, "Erro ao criar conexão: %d\n", cliente);
        return EXIT_FAILURE;
    }
    printf("Client QUIC criada com sucesso!\n");

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
    printf("Endereço local: %p, Tamanho: %d\n", path.local.addr, path.local.addrlen);
    printf("Endereço remoto: %p, Tamanho: %d\n", path.remote.addr, path.remote.addrlen);

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
    printf("Conexão recuperada: %p\n", (void *)retrieved_conn);

    //=====================================================criacao de um ctx pra conexao TLS
    WOLFSSL_CTX* ctx;
    wolfSSL_Init();
    printf("debug1\n");
    if((ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method())) == NULL){
        fprintf(stderr, "Erro ao criar o contexto do cliente\n");
        return EXIT_FAILURE;
    }
    printf("debug2\n");
    //==================================configuracao dos CA(teste)
    if (wolfSSL_CTX_load_verify_locations(ctx,"../cert/server.crt",0) !=
    SSL_SUCCESS) {
    fprintf(stderr, "Error loading cert/server.crt, please check"
    "the file. :/\n");
    exit(EXIT_FAILURE);
    }
    printf("debug3\n");\

    //=====================================utilizando a api crypto do ngtcp2 pra nn precisar derivar e instalar as chaves(com qualquer lib SSL)
    if(ngtcp2_crypto_wolfssl_configure_client_context(ctx)!=0){
        fprintf(stderr, "Erro ao configurar o contexto do cliente\n");
        return EXIT_FAILURE;
    }


    //=====================================================
    ngtcp2_conn_del(conn);
    return EXIT_SUCCESS;
}

