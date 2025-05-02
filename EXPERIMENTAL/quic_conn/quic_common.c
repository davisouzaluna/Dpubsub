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

