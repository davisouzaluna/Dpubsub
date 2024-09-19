#include "packets.h"


// Função para serializar um pacote CONNECT (client -> broker)
int serialize_connect(packet_type_code_t packet_type, char *buffer, size_t buffer_size, const char *client_id, uint16_t keep_alive, uint8_t version) {
    if (buffer == NULL || client_id == NULL) {
        return -1; // Erro: parâmetros inválidos
    }

    // Calcular o comprimento dos campos variáveis
    size_t client_id_len = strlen(client_id);
    size_t protocol_name_len = 4; // "MQTT"
    size_t variable_header_len = 2 + protocol_name_len + 1 + 1 + 2; // Tamanho base

    // Adicionar espaço para novos campos em MQTT 5.0, se necessário *NAO-FUNCIONAL-->SOMENTE A IDEIA*
    if (version == 0x05) {
        // Por exemplo, você poderia adicionar espaço para Session Expiry Interval
        variable_header_len += 2; // Exemplo: adicionar 2 bytes
    }

    size_t payload_len = 2 + client_id_len; // 2 bytes para client_id length + client_id
    size_t remaining_length = variable_header_len + payload_len;

    // Verificar se o buffer tem espaço suficiente
    if (2 + remaining_length > buffer_size) {
        return -2; // Erro: tamanho do buffer insuficiente
    }

    // Fixed Header
    buffer[0] = (packet_type << 4); // Tipo de pacote no alto de 4 bits
    buffer[1] = remaining_length;    // Comprimento restante

    // Variable Header
    size_t index = 2;

    // Protocol Name Length (2 bytes)
    buffer[index++] = 0x00;
    buffer[index++] = protocol_name_len;

    // Protocol Name ("MQTT")
    memcpy(buffer + index, "MQTT", protocol_name_len);
    index += protocol_name_len;

    // Protocol Level
    buffer[index++] = version;

    // Connect Flags
    uint8_t connect_flags = 0x00; // Ajuste conforme a necessidade
    if (version == 0x05) {
        connect_flags |= 0x01; // Exemplo: setar um flag para MQTT 5.0
    }
    buffer[index++] = connect_flags;

    // Keep Alive (2 bytes)
    buffer[index++] = (keep_alive >> 8) & 0xFF; // MSB
    buffer[index++] = keep_alive & 0xFF;        // LSB

    // Payload
    // Client ID Length (2 bytes)
    buffer[index++] = (client_id_len >> 8) & 0xFF; // MSB
    buffer[index++] = client_id_len & 0xFF;        // LSB

    // Client ID
    memcpy(buffer + index, client_id, client_id_len);
    index += client_id_len;

    // Retorna o tamanho total do pacote serializado
    return index;
}


int serialize_connack(packet_type_code_t packet_type, char *buffer, size_t buffer_size, uint8_t session_present, uint8_t return_code) {
    if (buffer == NULL || buffer_size < 2) {
        return -1;  // Buffer inválido ou tamanho insuficiente
    }

    // Verifica se o return_code é válido
    if (return_code > 0x05) {
        return -2;  // Código de retorno inválido
    }

    // Preenche o buffer com o tipo de pacote (CONNACK é 0x20)
    buffer[0] = packet_type;  // Aqui, packet_type deve ser 0x20 para CONNACK

    // O segundo byte é o campo "Flags" (session_present bit) e o código de retorno
    buffer[1] = (session_present << 1) | (return_code & 0x0F);

    return 2; 
}
