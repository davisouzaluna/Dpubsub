/*
Athor: Davi Souza de Luna
Description: This code implement the most common functions for server and client 
using the ngtcp2 library.
*/
#include <ngtcp2/ngtcp2.h>
#include <wolfssl/options.h>
#include <wolfssl/ssl.h> 
#include <wolfssl/quic.h>

/*
Generate a Connection ID(CID).
*/
void generate_cid(ngtcp2_cid *cid, size_t len);

/*
Create a new ngtcp2 path.
*/
ngtcp2_path *init_ngtcp2_path(const char *local_ip, uint16_t local_port,const char *remote_ip, uint16_t remote_port);

/*
free the ngtcp2 path.
*/
void free_ngtcp2_path(ngtcp2_path *path);

/*
Define default callbacks for ngtcp2.
*/
void define_callbacks(ngtcp2_callbacks *callback, void *user_data);

/*
Create the scruct thats allocate the timestamp to the ngtcp2_tstamp type, which is a uint64_t
*/
ngtcp2_tstamp timestamp_now();

/*
Create the wolfssl context in client.
*/
void create_wssl_init_api_client(WOLFSSL_CTX* ctx,WOLFSSL_QUIC_METHOD quic_method);

/*
Create the wolfssl context in server.
*/
void create_wssl_init_api_server(WOLFSSL_CTX* ctx,WOLFSSL_QUIC_METHOD quic_method);

