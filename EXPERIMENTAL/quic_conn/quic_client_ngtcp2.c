#include <ngtcp2/ngtcp2.h>
#include <stdio.h>
#include <ngtcp2/ngtcp2_crypto_wolfssl.h>
#include <ngtcp2/ngtcp2_crypto_wolfssl.h>


int main(){

    typedef struct {
    ngtcp2_conn *conn;
    const ngtcp2_cid *dcid;
    const ngtcp2_cid *scid;
    const ngtcp2_path *path;
    uint32_t client_chosen_version;
    const ngtcp2_callbacks *callbacks;
    ngtcp2_settings settings;
    const ngtcp2_transport_params *transport_params;
    const ngtcp2_mem *mem;
    void *user_data;
} quic_client;

    quic_client client_config;

    ngtcp2_settings_default(&client_config.settings);
    ngtcp2_transport_params_default(client_config.transport_params);
    
    client_config.settings.initial_ts = 0;
    client_config.client_chosen_version = NGTCP2_PROTO_VER_V1;

    
    
    //int tls_ctx;
    //tls_ctx = ngtcp2_crypto_wolfssl_configure_client_context(&client_config.conn);

    if(
    ngtcp2_conn_client_new(&client_config.conn, &client_config.dcid, &client_config.scid, &client_config.path, 
    client_config.client_chosen_version, 
    &client_config.callbacks, &client_config.settings, 
    client_config.transport_params, client_config.mem, 
    client_config.user_data)!=0){
        printf("Erro ao criar conexão cliente.\n");
    }

    printf("Conexão cliente criada com sucesso!\n");


    printf("Hello, World!\n");
}