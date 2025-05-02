#ifndef QUIC_COMMON_H
#define QUIC_COMMON_H
/*
Athor: Davi Souza de Luna
Description: This code implement the most common functions for server and client 
using the ngtcp2 library.
*/
#include <ngtcp2/ngtcp2.h>
#include <wolfssl/options.h>
#include <wolfssl/ssl.h> 
#include <wolfssl/quic.h>
#include <ngtcp2/ngtcp2_crypto.h>
#include <sys/time.h>
#include <time.h>
#define _POSIX_C_SOURCE 200809L

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
generate random numbers for ngtcp2.
*/
void custom_ngtcp2_rand(uint8_t *dest, size_t destlen, const ngtcp2_rand_ctx *rand_ctx);

/*
This function is simmilar to generate_cid, but it is used to generate a new CID
when the client sends a new connection ID(callback).
*/
int get_new_cid(ngtcp2_conn *conn, ngtcp2_cid *cid, uint8_t *token, size_t cidlen, void *user_data);
/*
Define default callbacks for ngtcp2.
*/
void define_callbacks(ngtcp2_callbacks *callback, void *user_data);

/*
Create the scruct thats allocate the timestamp to the ngtcp2_tstamp type, which is a uint64_t
*/
ngtcp2_tstamp timestamp_now();

ngtcp2_conn *my_get_conn(ngtcp2_crypto_conn_ref *ref);

/*
Create the wolfssl context in client.
*/
void create_wssl_init_api_client(WOLFSSL_CTX* ctx,WOLFSSL_QUIC_METHOD quic_method);

/*
Create the wolfssl context in server.
*/
void create_wssl_init_api_server(WOLFSSL_CTX* ctx,WOLFSSL_QUIC_METHOD quic_method);

#endif /* QUIC_COMMON_H */  
