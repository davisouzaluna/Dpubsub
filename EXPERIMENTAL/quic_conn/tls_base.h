/*
Author: Davi Souza de Luna
Date: 2024-04-25
Description: This code implement a header for TLS 1.3-QUIC context
*/

#include <wolfssl/options.h>
#include <wolfssl/ssl.h> 
#include <wolfssl/quic.h>

// create  ssl context for QUIC
int create_tls_ctx(WOLFSSL_CTX** ctx, WOLFSSL_QUIC_METHOD quic_method) {
}

/*
set the encryption secrets for TLS 1.3-QUIC. Is used to Callback(ngtcp2)
*/
int set_encryption_secrets(WOLFSSL* ssl, WOLFSSL_ENCRYPTION_LEVEL level,
    const uint8_t* read_secret,
    const uint8_t* write_secret,
    size_t secret_len) {
}

/*
Clear the flight of packets stored in buffer
*/
int flush_flight(WOLFSSL* ssl) {
}

/*
send alert for TLS 1.3-QUIC
*/
int send_alert(WOLFSSL* ssl, WOLFSSL_ENCRYPTION_LEVEL level,
    uint8_t alert) {
}