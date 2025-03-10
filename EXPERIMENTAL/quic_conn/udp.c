#include "udp.h"

int create_udp_socket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar socket UDP");
        return -1;
    }
    return sockfd;
}

int bind_udp_socket(int sockfd, const char *ip, int port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Erro ao vincular o socket");
        return -1;
    }

    return 0;
}

ssize_t send_udp_packet(int sockfd, const void *data, size_t data_len, const char *dest_ip, int dest_port) {
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(dest_port);
    dest_addr.sin_addr.s_addr = inet_addr(dest_ip);

    ssize_t sent_len = sendto(sockfd, data, data_len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if (sent_len < 0) {
        perror("Erro ao enviar pacote UDP");
        return -1;
    }

    return sent_len;
}

ssize_t receive_udp_packet(int sockfd, void *buffer, size_t buffer_len, struct sockaddr_in *src_addr) {
    socklen_t addr_len = sizeof(*src_addr);
    ssize_t recv_len = recvfrom(sockfd, buffer, buffer_len, 0, (struct sockaddr*)src_addr, &addr_len);
    if (recv_len < 0) {
        perror("Erro ao receber pacote UDP");
        return -1;
    }
    return recv_len;
}

void close_udp_socket(int sockfd) {
    close(sockfd);
}
