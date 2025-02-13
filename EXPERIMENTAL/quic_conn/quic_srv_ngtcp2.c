#include <ngtcp2/ngtcp2_crypto.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ngtcp2/ngtcp2.h>

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
    ngtcp2_path path;
    path.local.addr = NULL;  
    path.local.addrlen = 0;
    path.remote.addr = NULL;
    path.remote.addrlen = 0;

    generate_cid(&dcid, DCID_LEN);
    generate_cid(&scid, DCID_LEN);


    params.original_dcid = dcid;
    //If is a server, this param(original_dcid) is True
    params.original_dcid_present = 1;

    srv = ngtcp2_conn_server_new(&conn,&dcid,&scid,&path,client_chosen_version,&callbacks,&settings,&params,NULL,NULL);

    if (srv != 0) {
        fprintf(stderr, "Erro ao criar conexão: %d\n", srv);
        return EXIT_FAILURE;
    }
    printf("Server QUIC criada com sucesso!\n");

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

    printf("Versão escolhida do srv: 0x%08x\n", client_chosen_version);
    printf("Resultado da criação da conexão: %d\n", srv);

    printf("settings.cc/ algoritmo: %u\n", settings.cc_algo); //Se printar 1 significa que eh CUBIC
    



    //=====================================================
    ngtcp2_conn_del(conn);
    return EXIT_SUCCESS;
}

