#include "tls_base.h"

int create_tls_ctx(WOLFSSL_CTX** ctx, WOLFSSL_QUIC_METHOD quic_method) {
    wolfSSL_Init();
    *ctx = wolfSSL_CTX_new(wolfTLSv1_3_client_method());
    if (*ctx == NULL) {
        fprintf(stderr, "Error creating client context\n");
        return -1;
    }
    if (wolfSSL_CTX_set_quic_method(*ctx, &quic_method) != WOLFSSL_SUCCESS) {
        fprintf(stderr, "Error configuring QUIC method\n");
        return -1;
    }
    return 0;
}

int set_encryption_secrets(WOLFSSL* ssl, WOLFSSL_ENCRYPTION_LEVEL level,
    const uint8_t* read_secret,
    const uint8_t* write_secret,
    size_t secret_len) {
    
    return WOLFSSL_SUCCESS;
}
int flush_flight(WOLFSSL* ssl) {
    // Implementação da função para limpar o buffer de pacotes
    return WOLFSSL_SUCCESS;
}
int send_alert(WOLFSSL* ssl, WOLFSSL_ENCRYPTION_LEVEL level,
    uint8_t alert) {
    // Implementação da função para enviar um alerta
    printf("Sending QUIC alert (level %d): %d\n", level, alert);
    // Aqui você pode implementar a lógica para enviar o alerta QUIC
    // Isso pode envolver o uso de funções específicas da biblioteca QUIC
    // ou a manipulação direta do contexto QUIC.
    return WOLFSSL_SUCCESS;
}